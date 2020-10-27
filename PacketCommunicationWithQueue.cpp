/**
 * @file PacketCommunicationWithQueue.cpp
 * @author Jan Wielgus
 * @date 2020-08-22
 * 
 */

#include "PacketCommunicationWithQueue.h"


PacketCommunicationWithQueue::PacketCommunicationWithQueue(ITransceiver* lowLevelComm, size_t maxQueuedBuffers, uint8_t maxReceivingFailures)
    : NoQueuePacketCommunication(lowLevelComm, maxReceivingFailures), MaxQueuedBuffers(maxQueuedBuffers)
{
}


PacketCommunicationWithQueue::~PacketCommunicationWithQueue()
{
    deleteAllQueuedBuffers();
}


void PacketCommunicationWithQueue::execute()
{
    receiveIncomingBuffersToQueue();
    updateReceiveDataPacketsWithOldestBuffers();
}



void PacketCommunicationWithQueue::receiveIncomingBuffersToQueue()
{
    uint8_t failureCounter = 0;

    while (LowLevelComm->available() && failureCounter <= MaxReceivingFailures)
    {
        const DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
        if (receivedBuffer.size == 0)
        {
            failureCounter++;
            continue;
        }

        IDataPacket* destinationDataPacket = getReceiveDataPacketPointer(receivedBuffer.buffer[0], receivedBuffer.size - 1);
        if (destinationDataPacket == nullptr) // Check if packet of this id and size exist on the receive packets pointers list
        {
            failureCounter++;
            continue;
        }
        
        DataBuffer receivedBufferCopy;
        receivedBufferCopy.buffer = new uint8_t[receivedBuffer.size];
        receivedBufferCopy.size = receivedBuffer.size;
        copyBufferData(receivedBufferCopy, receivedBuffer);
        enqueueBuffer(receivedBufferCopy);
    }
}


void PacketCommunicationWithQueue::updateReceiveDataPacketsWithOldestBuffers()
{
    // TODO: implement this method

    // remember to release dynamically allocated memory !!
}



void PacketCommunicationWithQueue::enqueueBuffer(DataBuffer buffer)
{
    if (queuedBuffersList.getSize() >= MaxQueuedBuffers)
        deleteOldestBuffer();

    queuedBuffersList.add(buffer);
}


void PacketCommunicationWithQueue::deleteAllQueuedBuffers()
{
    while (queuedBuffersList.getSize() > 0)
        deleteOldestBuffer();
}


void PacketCommunicationWithQueue::deleteOldestBuffer()
{
    delete[] queuedBuffersList.get(0).buffer;
    queuedBuffersList.remove(0);
}
