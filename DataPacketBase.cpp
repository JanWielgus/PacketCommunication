/*
    Created:	09/04/2020
    Author:     Jan Wielgus
*/

#include <DataPacketBase.h>


DataPacketBase::DataPacketBase(uint8_t id)
    : ID(id)
{
}


DataPacketBase::~DataPacketBase()
{
}


void DataPacketBase::add_int8_t(int8_t& toAdd)
{
    // cast int8_t* to uint8_t* and then add to the byte pointers array
    uint8_t* ptrToAdd = (uint8_t*)(&toAdd);
    bytePointersArray.add(ptrToAdd);
}


void DataPacketBase::add_uint8_t(uint8_t& toAdd)
{
    bytePointersArray.add(&toAdd);
}


void DataPacketBase::addByteType(ByteType& toAdd)
{
    for (int i = 0; i < toAdd.AmtOfBytes; i++)
        bytePointersArray.add(&(toAdd.byteArr()[i]));
}


uint8_t DataPacketBase::getPacketID() const
{
    return ID;
}


uint8_t DataPacketBase::getPacketSize() const
{
    return bytePointersArray.getSize();
}


uint8_t** DataPacketBase::getBytePointersArray()
{
    return bytePointersArray.getArray();
}


const uint8_t** DataPacketBase::getBytePointersArray() const
{
    return (const uint8_t**)bytePointersArray.getArray();
}


void DataPacketBase::setPacketEvent(FC_Task* packetEvent)
{
    this->packetEvent = packetEvent;
}


FC_Task* DataPacketBase::getPacketEvent()
{
    return packetEvent;
}
