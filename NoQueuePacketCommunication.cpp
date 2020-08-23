/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

#include "NoQueuePacketCommunication.h"
#include "C:\Users\janwi\Documents\Arduino\libraries\DataStructures\SimpleGrowingArray.h" // FIXME: remove full path


NoQueuePacketCommunication::NoQueuePacketCommunication(ITransceiver* lowLevelComm)
    : PacketCommunication(lowLevelComm)
{
    receiveDataPacketsPointers = new SimpleGrowingArray<IDataPacket*>();
}


NoQueuePacketCommunication::~NoQueuePacketCommunication()
{
    delete receiveDataPacketsPointers;
}


bool NoQueuePacketCommunication::addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr)
{
    if (checkIfAlreadyAdded(receiveDataPacketPtr))
        return false;
    
    receiveDataPacketsPointers->add(receiveDataPacketPtr);
    return true;
}


bool NoQueuePacketCommunication::sendDataPacket(const IDataPacket* packetToSend)
{
    // buffer[0] - packet ID
    // buffer[1, 2, ...] - data

    // create buffer
    DataBuffer dataToSend;
    dataToSend.size = packetToSend->getPacketSize() + 1; // data + packet ID
    dataToSend.buffer = new uint8_t[dataToSend.size];

    // add packet ID
    dataToSend.buffer[0] = packetToSend->getPacketID();

    // copy data
    const uint8_t** packetDataPointersArray = packetToSend->getBytePointersArray();
    for (int i = 1; i < dataToSend.size; i++)
        dataToSend.buffer[i] = *(packetDataPointersArray[i - 1]);

    bool sendingResult = LowLevelComm->send(dataToSend);
    delete[] dataToSend.buffer;
    return sendingResult;
}


void NoQueuePacketCommunication::execute()
{
    receiveDataAndUpdateReceiveDataPackets();
}



void NoQueuePacketCommunication::receiveDataAndUpdateReceiveDataPackets()
{
    bool someDataReceivedFlag = false;

    while (LowLevelComm->available())
    {
        DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
        if (receivedBuffer.size == 0)
            continue;
        
        IDataPacket* destinationDataPacket = getReceiveDataPacketPointer(receivedBuffer.buffer[0], receivedBuffer.size - 1);
        if (destinationDataPacket == nullptr)
            continue; // any previously added receive data packets match received data

        updateDataInDataPacket(destinationDataPacket, receivedBuffer);
        callPacketEvent(destinationDataPacket);

        someDataReceivedFlag = true;
    }

    updateConnectionStability(someDataReceivedFlag * 100); // 100 when true, 0 when false
}


bool NoQueuePacketCommunication::checkIfAlreadyAdded(IDataPacket* toCheck)
{
    uint8_t toCheckID = toCheck->getPacketID();
    for (int i = 0; i < receiveDataPacketsPointers->getSize(); i++)
        if (receiveDataPacketsPointers[i]->getPacketID() == toCheckID)
            return true;
    return false;
}


IDataPacket* NoQueuePacketCommunication::getReceiveDataPacketPointer(uint8_t packetID, size_t packetSize)
{
    for (int i = 0; i < receiveDataPacketsPointers.getSize(); i++)
    {
        IDataPacket* currentlyCheckedPacket = receiveDataPacketsPointers[i];
        if (currentlyCheckedPacket->getPacketID() == packetID &&
            currentlyCheckedPacket->getPacketSize() == packetSize)
            return currentlyCheckedPacket;
    }

    return nullptr;
}


void NoQueuePacketCommunication::updateDataInDataPacket(IDataPacket* dataPacket, DataBuffer sourceDataBuffer)
{
    uint8_t** destinationBytePointers = dataPacket->getBytePointersArray();
    for (int i = 1; i < sourceDataBuffer.size; i++)
        *(destinationBytePointers[i - 1]) = sourceDataBuffer.buffer[i];
}


void NoQueuePacketCommunication::callPacketEvent(IDataPacket* dataPacket)
{
    IExecutable* packetEvent = dataPacket->getPacketEventPtr();
    if (packetEvent != nullptr)
        packetEvent->execute();
}
