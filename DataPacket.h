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
#include <IByteType.h>
#include <GrowingArray.h>


namespace PacketComm
{
    class DataPacket : public Packet
    {
        SimpleDataStructures::GrowingArray<IByteType*> byteTypeArray;
        size_t dataOnlySize = 0;


    public:
        /**
         * @brief Ctor.
         * @param packetID Unique ID of the packet.
         * @param onReceiveCallback (optional) pointer to void function
         * that will be called each time after receiving this packet.
         */
        explicit DataPacket(PacketIDType packetID, Callback onReceiveCallback = nullptr);

        /**
         * @brief Add next variable to this data packet (add all variables
         * that this data packet consists on program setup).
         * @param toAdd Object that extends IByteType
         */
        void addVar(IByteType& toAdd);

    protected:
        size_t getDataOnly(uint8_t* outputBuffer) const override;
        size_t getDataOnlySize() const override;
        void updateDataOnly(const uint8_t* inputBuffer) override;
    };
}


#endif
