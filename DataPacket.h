/**
 * @file DataPacket.h
 * @author Jan Wielgus
 * @brief Concrete class that is used to send multiple
 * variables of different types in form of data packets.
 * @date 2020-04-22
 */

#ifndef BYTEPACKET_H
#define BYTEPACKET_H

#include "Packet.h"


namespace PacketComm
{
    class DataPacket : public Packet
    {
        uint8_t* payload;
        size_t payloadSize = 0;

    public:
        /**
         * @param packetID Unique ID of the packet.
         * @param payload Pointer to the source/destination data.
         * @param payloadSize Size of the source/destination data in bytes.
         * @param onReceiveCallback (optional) pointer to void function
         * that will be called each time after receiving this packet.
         */
        DataPacket(PacketIDType packetID, uint8_t* payload, size_t payloadSize, Callback onReceiveCallback = nullptr);

    protected:
        size_t getDataOnly(uint8_t* outputBuffer) const override;
        size_t getDataOnlySize() const override;
        void updateDataOnly(const uint8_t* inputBuffer) override;
    };
}


#endif
