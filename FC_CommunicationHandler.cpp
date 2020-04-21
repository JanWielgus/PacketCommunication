/*
    Created:	10/04/2020
    Author:     Jan Wielgus
*/

#include <FC_CommunicationHandler.h>


FC_CommunicationHandler::FC_CommunicationHandler(Stream* serial, uint8_t bufSize)
    : comBase(serial, bufSize)
{
}


FC_CommunicationHandler::~FC_CommunicationHandler()
{
    // Delete queues for each receive data packet
    for (int i = 0; i < receiveDataPacketsPointersArray.getSize(); i++)
    {
        // Delete unread memory allocated with buffers copy
        while (receiveDataPacketsPointersArray[i].queuePtr->getQueueLength() > 0)
            delete[] receiveDataPacketsPointersArray[i].queuePtr->dequeue().buffer;

        // Delete memory allocated for the queue
        delete receiveDataPacketsPointersArray[i].queuePtr;
    }
}


/*
    Set up a receive packet bundle:
    - copy pointer to the data packet where unpacked data will be stored
    - create a queue to store incoming packets of that type
*/
bool FC_CommunicationHandler::addRaceiveDataPacketPointer(ITransferable* recDPptr, uint8_t queuedPacketsAmount)
{
    // Check if pointer of data packet of this type was not already added
    // And return false if there was packet with the same ID
    for (int i = 0; i < receiveDataPacketsPointersArray.getSize(); i++)
        if (receiveDataPacketsPointersArray[i].packetPtr->getPacketID() == recDPptr->getPacketID())
            return false;

    // Make a pack to add
    receiveDataPacketBundle packetBundle;
    packetBundle.packetPtr = recDPptr;
    
    // Create a queue for this packet
    if (queuedPacketsAmount < 1)
        queuedPacketsAmount = 1;
    packetBundle.queuePtr = new dataPacketQueue(queuedPacketsAmount);

    // Add to the array
    receiveDataPacketsPointersArray.add(packetBundle);

    return true;
}


void FC_CommunicationHandler::execute()
{
    // Put all incoming data to the proper queues
    bool ifReceivedAnyData = receivePacketsToQueue();
    updateConnectionStability(ifReceivedAnyData);

    // Update receive data packets first came data
    dequeueOldestPacketOfEachType();
}


bool FC_CommunicationHandler::sendDataPacket(const ITransferable* packetToSend)
{
    // buffer[0] - checksum
    // buffer[1] - packet ID
    // buffer[2, 3, ...] - data
    
    // copy values to speed up
    uint8_t packetDataSize = packetToSend->getPacketSize();
    const uint8_t** packetDataArray = packetToSend->getBytePointersArray();

    // Set packet sizeOfData and packet ID
    comBase.dpToSend.size = (size_t)(packetDataSize + 2); // two additional bytes for ID and checksum
    comBase.dpToSend.buffer[1] = packetToSend->getPacketID();

    // Add all packet data bytes to the array that will be sent
    for (int i = 0; i < packetDataSize; i++)
        comBase.dpToSend.buffer[i + 2] = *(packetDataArray[i]);

    // calculate checksum and add it to the data packet
    comBase.dpToSend.buffer[0] = comBase.calcChecksum();

    comBase.sendData();

    return true; // I don't know what could went wrong, so return always true
}


uint8_t FC_CommunicationHandler::getConnectionStability()
{
    return (uint8_t)(conStabFilter.getLastValue() + 0.5);
}


void FC_CommunicationHandler::adaptConStabFilterToInterval()
{
    // Thanks to this, filtering is not dependent of receiving time interval
    // Just a linear funciton that for 20000 interval return 0.85 and for 500000 return 0.5 and so on
    // Calculated using reglinp function in Excel for this two points
    float temp = constrain(-7.3e-7 * (float)getInterval() + 0.86f, 0.2f, 0.95f);
    conStabFilter.setFilterBeta(temp);
}


/*
    Returns true if at least one packet was received
    All packets in the queue have valid checksum (which is currentlyChecked before enqueuing)
*/
bool FC_CommunicationHandler::receivePacketsToQueue()
{
    bool receivedSomeDataFlag = false;

    while (comBase.receiveData()) // while there are new data packets
    {
        // Check if packet is valid (calculate the checksum)
        if (!comBase.checkChecksum())
            continue;

        // Check the packet type
        uint8_t currentPacketIndex = 255;
        for (int i = 0; i < receiveDataPacketsPointersArray.getSize(); i++)
        {
            ITransferable* currentlyChecked = receiveDataPacketsPointersArray[i].packetPtr;
            if (currentlyChecked->getPacketID() == comBase.dpReceived.buffer[1]
                && currentlyChecked->getPacketSize() == (comBase.dpReceived.size - 2) ) // excluding packet ID and checksum
                currentPacketIndex = i;
        }

        // If didn't found such a receive data packet in the array
        if (currentPacketIndex == 255)
            continue;

        // Set flag that indicate that any packet was received
        receivedSomeDataFlag = true;


        // HINT !
        // there you can check if queue of the current packet type is empty
        // and if so, do not allocate new memory and store data directly to the receive packet


        // To speed up, create queue pointer
        dataPacketQueue* curQueue = receiveDataPacketsPointersArray[currentPacketIndex].queuePtr;

        // If the queue is full, free memory allocated by the oldest buffer copy
        if (curQueue->isFull())
            delete[] curQueue->dequeue().buffer;

        // Make a deep copy of recived buffer
        dataBufferType receivedPacket;
        receivedPacket.size = comBase.dpReceived.size;
        receivedPacket.buffer = new uint8_t[receivedPacket.size]; // dynamically allocate memory for buffer copy
        for (int i = 0; i < receivedPacket.size; i++)
            receivedPacket.buffer[i] = comBase.dpReceived.buffer[i];

        // Add copied buffer to the queue of this packet type
        curQueue->enqueue(receivedPacket);
    }

    return receivedSomeDataFlag;
}


void FC_CommunicationHandler::dequeueOldestPacketOfEachType()
{
    receiveDataPacketBundle currentBundle;

    for (int i = 0; i < receiveDataPacketsPointersArray.getSize(); i++)
    {
        // Just to speed up
        currentBundle = receiveDataPacketsPointersArray[i];

        // Skip this packet if it don't have any new data
        if (currentBundle.queuePtr->getQueueLength() == 0)
            continue;

        uint8_t** outputDataBytePointers = currentBundle.packetPtr->getBytePointersArray(); // array of pointers to bytes where data will be stored
        dataBufferType sourceDataBuffer = currentBundle.queuePtr->dequeue();
        uint8_t sizeOfData = sourceDataBuffer.size - 2; // excluding packet ID and packet checksum
        
        // Update data in the receive data packet
        for (int i = 0; i < sizeOfData; i++)
            *(outputDataBytePointers[i]) = sourceDataBuffer.buffer[i + 2];

        // Call packet event if exist
        FC_Task* packetEvent;
        if ( (packetEvent = currentBundle.packetPtr->getPacketEvent()) != nullptr)
            packetEvent->execute();

        // Delete the memory allocated for the buffer
        delete[] sourceDataBuffer.buffer;
    }
}


void FC_CommunicationHandler::updateConnectionStability(bool receivedDataFlag)
{
    conStabFilter.updateFilter(receivedDataFlag ? 100.0f : 0.0f);
}
