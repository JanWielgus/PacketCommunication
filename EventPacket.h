/**
 * @file EventPacket.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief This is a data packet type that don't contain any data
 * but only a packet received event. Is used to
 * trigger some action.
 * @date 2021-01-28
 * 
 */


#ifndef EVENTPACKET_H
#define EVENTPACKET_H

#include "Packet.h"


namespace PacketComm
{
    /**
     * @brief This is a data packet type that don't contain any data 
     * but only a packet received event. Is used to
     * trigger some action.
     */
    class EventPacket : public Packet
    {
    public:
        explicit EventPacket(PacketIDType packetID, Callback onReceiveCallback = nullptr)
            : Packet(packetID, Type::EVENT, onReceiveCallback)
        {
        }


    protected:
        size_t getDataOnly(uint8_t*) const override
        {
            return 0;
        }

        size_t getDataOnlySize() const override
        {
            return 0;
        }

        void updateDataOnly(const uint8_t*) override
        {
        }
    };
}


#endif