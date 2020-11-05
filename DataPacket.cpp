/**
 * @file DataPacket.cpp
 * @author Jan Wielgus
 * @date 2020-08-06
 * 
 */

#include "DataPacket.h"
#include <GrowingArray.h>


DataPacket::DataPacket(uint8_t packetID)
    : PacketID(packetID)
{
    bytePointersArray = new GrowingArray<uint8_t*>(); // default array type
}


DataPacket::DataPacket(uint8_t packetID, IArray<uint8_t*>* arrayPointer)
    : PacketID(packetID)
{
    bytePointersArray = arrayPointer;
    useExternalArray = true;
}


DataPacket::~DataPacket()
{
    if (!useExternalArray && bytePointersArray != nullptr)
        delete bytePointersArray;
}


void DataPacket::addByteType(IByteType& toAdd)
{
    for (int i = 0; i < toAdd.byteSize(); i++)
        bytePointersArray->add(&(toAdd.byteArray()[i]));
}


uint8_t DataPacket::getPacketID() const
{
    return PacketID;
}


size_t DataPacket::getPacketSize() const
{
    return bytePointersArray->getSize();
}


uint8_t** DataPacket::getBytePointersArray()
{
    return bytePointersArray->toArray();
}


const uint8_t** DataPacket::getBytePointersArray() const
{
    return (const uint8_t**)bytePointersArray->toArray();
}


void DataPacket::setPacketReceivedEvent(IExecutable& packetReceivedEvent)
{
    this->packetReceivedEvent = &packetReceivedEvent;
}


IExecutable* DataPacket::getPacketEventPtr() const
{
    return packetReceivedEvent;
}
