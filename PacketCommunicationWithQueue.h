/**
 * @file PacketCommunicationWithQueue.h
 * @author Jan Wielgus
 * @brief Concrete class that extend NoQueuePacketCommunication class.
 * This class differs from NoQueuePacketCommunication class only in receiving.
 * Extends it with a queue for received data packets. In this class in one receiving session
 * at most one data packet of each type can be received (other are queued
 * and received one by one in next sessions).
 * @date 2020-08-19
 * 
 */

// FIXME: figure out better name that show, that this class has queue only for received data!!!

#ifndef PACKETCOMMUNICATIONWITHQUEUE_H
#define PACKETCOMMUNICATIONWITHQUEUE_H

#include "NoQueuePacketCommunication.h"
#include <LinkedList.h>


/**
 * @brief This class differs from NoQueuePacketCommunication class only in receiving.
 * Extends it with a queue for received data packets. In this class in one receiving session
 * at most one data packet of each type can be received (other are queued
 * and received one by one in next sessions).
 */
class PacketCommunicationWithQueue : public NoQueuePacketCommunication
{
protected:
    /// Buffers on the queue are dynamically allocated and need to be deleted after dequeue.
    /// At index 0 is the oldest buffer (first to receive).
    LinkedList<DataBuffer> queuedBuffersList;
    const size_t MaxQueuedBuffers;

public:
    PacketCommunicationWithQueue(ITransceiver* lowLevelComm, size_t maxQueuedBuffers, uint8_t maxReceivingFailures = DefaultMaxReceivingFailures);
    ~PacketCommunicationWithQueue();
    void execute() override;

protected:
    void receiveIncomingBuffersToQueue();
    void updateReceiveDataPacketsWithOldestBuffers();

private:
    /**
     * @brief Add buffer to the queuedBuffers queue.
     * Do not allocate memory. Just store the pointer.
     * 
     * @param buffer Buffer to put in the queue.
     */
    void enqueueBuffer(DataBuffer buffer);

    /**
     * @brief Dequeue all elements in the queue and release dynamically allocated memory for stored buffers.
     */
    void deleteAllQueuedBuffers();

    /**
     * @brief Removes from queue and deletes the oldest buffer memory.
     */
    void deleteOldestBuffer();
};


#endif
