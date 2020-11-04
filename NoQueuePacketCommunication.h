/**
 * @file NoQueuePacketCommunication.h
 * @author Jan Wielgus
 * @brief Concrete packet communication class that during
 * receiving automatically updates data in receive data packets.
 * All incoming data available is received in each receiving session
 * (execute() method call).
 * @date 2020-08-19
 * 
 */

#ifndef NOQUEUEPACKETCOMMUNICATION_H
#define NOQUEUEPACKETCOMMUNICATION_H

#include "PacketCommunication.h"


/**
 * @brief All incoming data available is received in each receiving session
 * (execute() method call).
 */
class NoQueuePacketCommunication : public PacketCommunication
{
protected:
    /// Size of this buffer is size of the biggest sent packet (increased gradually).
    /// If any packet was sent its size is 0 and buffer is nullptr. Used in sendDataPacket() method.
    DataBuffer sendingBuffer;

    /// Maximum amount of data packets with size = 0 received despite available() method returned true.
    /// Prevent from an infinite loop.
    const uint8_t MaxReceivingFailures;
    static const uint8_t DefaultMaxReceivingFailures;

public:
    NoQueuePacketCommunication(ITransceiver* lowLevelComm, uint8_t maxReceivingFailures = DefaultMaxReceivingFailures);
    virtual ~NoQueuePacketCommunication();
    bool sendDataPacket(const IDataPacket* packetToSend) override;
    virtual void execute() override;


    NoQueuePacketCommunication(const NoQueuePacketCommunication& other) = delete;
    NoQueuePacketCommunication& operator=(const NoQueuePacketCommunication& other) = delete;


private:
    /**
     * @brief If sendingBuffer size is less than minimumSize,
     * deallocate sendingBuffer and allocate new one with the minimumSize
     * (don't copy data from old array to new one).
     * @param minimumSize Minimum size of the sendingBuffer.
     */
    void ensureSendingBufferCapacity(size_t minimumSize); // this method could be made general for any buffer, but there is no need

    /**
     * @brief Updates data packets using all data available to receive.
     * Has protection against infinite loop.
     */
    void receiveDataAndUpdateReceiveDataPackets();
};


#endif
