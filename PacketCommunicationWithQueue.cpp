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
        
        DataBuffer receivedBufferCopy = getDynamicallyAllocatedBufferCopy(receivedBuffer);
        enqueueBuffer(receivedBufferCopy);
    }
}


// TODO: this method can be coded bette for sure, review it
void PacketCommunicationWithQueue::updateReceiveDataPacketsWithOldestBuffers()
{
    if (queuedBuffersList.isEmpty())
        return;


    size_t recPktPtrsSize = receiveDataPacketsPointers.getSize();
    bool* packetUpdatedFlags = new bool[recPktPtrsSize]; // If a place is true, packet of this ID was updated
    size_t updatedPackets = 0; // amount of true values in the packetUpdatedFlags array
    bool someDataReceivedFlag = false;

    for (size_t i = 0; i < recPktPtrsSize; i++)
        packetUpdatedFlags[i] = false;

    RemovingIterator<DataBuffer>* buffersIterator = queuedBuffersList.getRemovingIterator();
    while (buffersIterator->hasNext())
    {
        DataBuffer& checkedBuffer = buffersIterator->next();
        size_t destinationDataPacketIndex;
        IDataPacket* destinationDataPacket = getReceiveDataPacketPointer(checkedBuffer.buffer[0], checkedBuffer.size - 1, &destinationDataPacketIndex);


        // If this buffer don't match any dataPacket.
        // Theoretically this is unnecessary, because before putting packet on the list
        // it is checked if proper receive data packet exists.
        // So this block should never execute.
        if (destinationDataPacket == nullptr)
        {
            delete[] checkedBuffer.buffer;
            buffersIterator->remove();
            continue;
        }

        // if this dataPacket was already updated with new data in this loop (leave that data for another receiving)
        if (packetUpdatedFlags[destinationDataPacketIndex] == true)
            continue;


        updateDataPacketFromBuffer(destinationDataPacket, checkedBuffer);
        callPacketReceivedEvent(destinationDataPacket);

        delete[] checkedBuffer.buffer;
        buffersIterator->remove();

        packetUpdatedFlags[destinationDataPacketIndex] = true;
        updatedPackets++;

        someDataReceivedFlag = true;
        
        // If all packets was updated
        if (updatedPackets == recPktPtrsSize)
            break;
    }

    delete[] packetUpdatedFlags;

    updateConnectionStability((uint8_t)someDataReceivedFlag * 100);
}



DataBuffer PacketCommunicationWithQueue::getDynamicallyAllocatedBufferCopy(const DataBuffer& bufferToCopy)
{
    DataBuffer bufferCopy;
    bufferCopy.buffer = new uint8_t[bufferToCopy.size];
    bufferCopy.size = bufferToCopy.size;
    copyBufferData(bufferCopy, bufferToCopy);
    return bufferCopy;
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
    if (queuedBuffersList.getSize() == 0)
        return;

    delete[] queuedBuffersList.get(0).buffer;
    queuedBuffersList.remove(0);
}

