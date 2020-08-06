/**
 * @file DataPacket.h
 * @author Jan Wielgus
 * @brief Concrete class that implement IDataPacket interface.
 * Can be used to create data packets instances.
 * @date 2020-08-05
 * 
 */

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "IDataPacket.h"
#include <C:\Users\janwi\Documents\Arduino\libraries\DataStructures\IArray.h>


class DataPacket : public IDataPacket
{
private:
    const uint8_t PacketID;
    IArray<uint8_t*>* bytePointersArray;
    IExecutable* packetEvent;


public:
    DataPacket(uint8_t packetID);
    DataPacket(uint8_t packetID, IArray<uint8_t*>* arrayPointer);
    virtual ~DataPacket();

    void addByteType(IByteType& toAdd) override;
    uint8_t getPacketID() const override;
    size_t getPacketSize() const override;
    const uint8_t** getBytePointersArray() const override;
    void setPacketEvent(const IExecutable& packetEvent) override;
    IExecutable* getPacketEventPtr() const override;
};


#endif
