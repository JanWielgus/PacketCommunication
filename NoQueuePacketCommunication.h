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
    /// AllocatedSize of this buffer is size of the biggest sent packet.
    ExtendedDataBuffer sendingBuffer;

    /// Maximum amount of data packets with size = 0 received despite available() method returned true.
    /// Prevent from an infinite loop.
    const uint8_t MaxReceivingFailures;
    static const uint8_t DefaultMaxReceivingFailures;

public:
    NoQueuePacketCommunication(ITransceiver* lowLevelComm, uint8_t maxReceivingFailures = DefaultMaxReceivingFailures);
    virtual ~NoQueuePacketCommunication();
    bool sendDataPacket(const IDataPacket* packetToSend) override;
    virtual void receiveAndUpdatePackets() override;


    NoQueuePacketCommunication(const NoQueuePacketCommunication& other) = delete;
    NoQueuePacketCommunication& operator=(const NoQueuePacketCommunication& other) = delete;
};


#endif
