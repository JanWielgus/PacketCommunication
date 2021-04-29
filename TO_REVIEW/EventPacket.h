/**
 * @file EventPacket.h
 * @author Jan Wielgus
 * @brief This is a data packet type that don't contain any data
 * but only a packet received event. Is used to
 * trigger some action on the receiver.
 * @date 2021-01-28
 * 
 */

#ifndef EVENTPACKET_H
#define EVENTPACKET_H

#include "IDataPacket.h"


class EventPacket : public IDataPacket
{
    const uint8_t PacketID;
    IExecutable* packetReceivedEvent = nullptr;

public:
    EventPacket(uint8_t packetID)
        : PacketID(packetID)
    {
    }

    uint8_t getPacketID() const override
    {
        return PacketID;
    }

    // always return 0
    size_t getPacketSize() const override
    {
        return 0;
    }

    void setPacketReceivedEvent(IExecutable& packetReceivedEvent) override
    {
        this->packetReceivedEvent = &packetReceivedEvent;
    }

    IExecutable* getPacketReceivedEventPtr() const override
    {
        return packetReceivedEvent;
    }

    // no effect
    void addByteType(IByteType& toAdd) override {}

    // always return nullptr
    uint8_t** getBytePointersArray() override
    {
        return nullptr;
    }

    // always return nullptr
    const uint8_t** getBytePointersArray() const override
    {
        return nullptr;
    }
};


#endif
