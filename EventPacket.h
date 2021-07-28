/**
 * @file EventPacket.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief This is a data packet type that don't contain any data
 * and is used to trigger some action on receiving.
 * @date 2021-01-28
 */

#ifndef EVENTPACKET_H
#define EVENTPACKET_H

#include "Packet.h"


namespace PacketComm
{
    /**
     * @brief This is a data packet type that don't contain any data
     * and is used to trigger some action on receiving.
     */
    class EventPacket : public Packet
    {
    public:
        /**
         * @brief Ctor. Creaters EventPacket and sets it's ID and
         * on receive callback (if don't know yet set nullptr).
         * @param packetID ID of this packet.
         * @param onReceiveCallback callback void method.
         */
        EventPacket(PacketIDType packetID, Callback onReceiveCallback = nullptr)
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
