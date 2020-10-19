/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

// FIXME: REVIEW THIS FILE

#include "NoQueuePacketCommunication.h"
#include <GrowingArray.h>


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
    DataBuffer bufferToSend = createNewBufferAndAllocateMemory(packetToSend->getPacketSize() + 1);

    updateBufferFromDataPacket(bufferToSend, packetToSend);

    bool sendingResult = LowLevelComm->send(bufferToSend);
    delete[] bufferToSend.buffer;
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

        updateDataPacketFromBuffer(destinationDataPacket, receivedBuffer);
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
