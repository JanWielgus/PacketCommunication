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


namespace PacketComm
{
    class BytePacket : public Packet
    {
        SimpleDataStructures::GrowingArray<IByteType*> byteTypeArray;
        size_t dataOnlySize = 0;


    public:
        explicit BytePacket(uint16_t packetID, Callback onReceiveCallback = nullptr);

        void addByteType(IByteType& toAdd);

    protected:
        size_t getDataOnly(uint8_t* outputBuffer) const override;
        size_t getDataOnlySize() const override;
        void updateDataOnly(const uint8_t* inputBuffer) override;
    };
}


#endif
