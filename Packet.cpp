/**
 * @file Packet.cpp
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2021-04-26
 * 
 */

#include "Packet.h"


using namespace PacketComm;


Packet::Packet(PacketIDType packetID, Type type, Callback callback)
    : PacketID(packetID), packetType(type), onReceiveCallback(callback)
{
}


Packet::~Packet()
{
}


void Packet::setOnReceiveCallback(Callback callback)
{
    onReceiveCallback = callback;
}


size_t Packet::getBuffer(uint8_t* outputBuffer) const
{
    uint8_t packetIDSize = PacketID.byteSize();
    const uint8_t* packetIDArray = PacketID.byteArray();
    for (uint8_t i = 0; i < packetIDSize; ++i)
        outputBuffer[i] = packetIDArray[i];
    
    return packetIDSize + getDataOnly(outputBuffer + packetIDSize);
}


bool Packet::updateBuffer(const uint8_t* inputBuffer)
{
    if (!checkIfBufferMatch(inputBuffer))
        return false;

    updateDataOnly(inputBuffer + PacketID.byteSize());
    return true;
}


Packet::PacketIDType Packet::getIDFromBuffer(const uint8_t* buffer)
{
    byteType<PacketIDType> id;
    for (uint8_t i = 0; i < id.byteSize(); ++i)
        id.byteArray()[i] = buffer[i];
    
    return (PacketIDType)id;
}


bool Packet::checkIfBufferMatch(const uint8_t* buffer)
{
    for (uint8_t i = 0; i < PacketID.byteSize(); ++i)
        if (buffer[i] != PacketID.byteArray()[i])
            return false;
    return true;
}
