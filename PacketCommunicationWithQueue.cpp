/**
 * @file PacketCommunicationWithQueue.cpp
 * @author Jan Wielgus
 * @date 2020-08-22
 * 
 */

#include "PacketCommunicationWithQueue.h"
#include "C:\Users\janwi\Documents\Arduino\libraries\DataStructures\SinkingQueue.h" // FIXME: remove full path


PacketCommunicationWithQueue::PacketCommunicationWithQueue(ITransceiver* lowLevelComm, size_t maxQueuedBuffers)
    : NoQueuePacketCommunication(lowLevelComm)
{
    queuedBuffers = new SinkingQueue(maxQueuedBuffers);
}


PacketCommunicationWithQueue::~PacketCommunicationWithQueue()
{
    clearQueueAndFreeBuffersMemory();
    delete queuedBuffers;
}


void PacketCommunicationWithQueue::execute()
{
    receiveIncomingBuffersToQueue();
    updateReceiveDataPacketsWithOldestBuffers();
}



void PacketCommunicationWithQueue::receiveIncomingBuffersToQueue()
{
    while (LowLevelComm->available())
    {
        DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
        if (receivedBuffer.size == 0)
            continue;

        // Check if packet of this id and size exist on the receive packets pointers list
        if (getReceiveDataPacketPointer(receivedBuffer.buffer[0], receivedBuffer.size - 1) == nullptr)
            continue;
        
        DataBuffer receivedBufferCopy = createNewBufferAndAllocateMemory(receivedBuffer.size);
        copyBufferContents(receivedBuffer, receivedBufferCopy);
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
    if (queuedBuffers->isFull())
        delete[] queuedBuffers->dequeue().buffer;
    
    queuedBuffers->enqueue(buffer);
}


void PacketCommunicationWithQueue::clearQueueAndFreeBuffersMemory()
{
    while (queuedBuffers->getQueueLength() > 0)
        delete[] queuedBuffers->dequeue().buffer;
}
