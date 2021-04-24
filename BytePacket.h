/**
 * @file BytePacket.h
 * @author Jan Wielgus
 * @brief Concrete class that implement IDataPacket interface.
 * Can be used to create data packets instances.
 * @date 2020-04-22
 * 
 */

#ifndef BYTEPACKET_H
#define BYTEPACKET_H

#include "Packet.h"
#include <IByteType.h>
#include <GrowingArray.h>


namespace PacketCommunication
{
    class BytePacket : public Packet
    {
        SimpleDataStructures::GrowingArray<IByteType*> byteTypeArray;


    public:
        BytePacket(uint8_t packetID);

        void addByteType(IByteType& toAdd);

        size_t getSize() const override;
        size_t getBuffer(uint8_t* outputBuffer) const override;
        void updateBuffer(const uint8_t* inputBuffer) override;
    };
}


#endif
