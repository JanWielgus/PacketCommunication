/**
 * @file DataPacket.cpp
 * @author Jan Wielgus
 * @date 2020-08-06
 * 
 */

#include "DataPacket.h"
#include <SimpleGrowingArray.h>


DataPacket::DataPacket(uint8_t packetID)
    : PacketID(packetID)
{
    bytePointersArray = new SimpleGrowingArray();
    packetEvent = nullptr;
}


DataPacket::DataPacket(uint8_t packetID, IArray<uint8_t*>* arrayPointer)
{
    bytePointersArray = arrayPointer;
    packetEvent = nullptr;
}


DataPacket::~DataPacket()
{
    if (bytePointersArray != nullptr)
        delete bytePointersArray;
}


void DataPacket::addByteType(IByteType& toAdd)
{
    for (int i = 0; i < toAdd.byteSize(); i++)
        bytePointersArray.add(&(toAdd.byteArray()[i]));
}


uint8_t DataPacket::getPacketID() const
{
    return PacketID;
}


size_t DataPacket::getPacketSize() const
{
    return bytePointersArray.getSize();
}


const uint8_t** DataPacket::getBytePointersArray() const
{
    return (const uint8_t**)bytePointersArray.toArray();
}


void DataPacket::setPacketEvent(const IExecutable& packetEvent)
{
    this->packetEvent = &packetEvent;
}


IExecutable* DataPacket::getPacketEventPtr() const
{
    return packetEvent;
}
