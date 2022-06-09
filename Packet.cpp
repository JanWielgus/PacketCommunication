/**
 * @file Packet.cpp
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2021-04-26
 */

#include "Packet.h"

using namespace PacketComm;


Packet::Packet(PacketIDType packetID, Type type, Callback callback)
    : PacketID(packetID), packetType(type), onReceiveCallback(callback)
{
}


void Packet::setOnReceiveCallback(Callback callback)
{
    onReceiveCallback = callback;
}


size_t Packet::getBuffer(uint8_t* outputBuffer) const
{
    // MSB is first (Big-endian)
    PacketIDType id = PacketID;
    // copy byte by byte from last to first
    for (uint8_t i = sizeof(PacketIDType) - 1; i >= 0; --i)
    {
        outputBuffer[i] = uint8_t(id & 0xff);
        id >>= 8;
    }

    return sizeof(PacketIDType) + getDataOnly(outputBuffer + sizeof(PacketIDType));
}


bool Packet::updateBuffer(const uint8_t* inputBuffer)
{
    if (!checkIfBufferMatch(inputBuffer))
        return false;

    updateDataOnly(inputBuffer + sizeof(PacketIDType));
    return true;
}


Packet::PacketIDType Packet::getIDFromBuffer(const uint8_t* buffer)
{
    // MSB is first (Big-endian)
    PacketIDType id = 0;
    for (uint8_t i = 0; i < sizeof(PacketIDType); ++i)
    {
        id <<= 8;
        id |= buffer[i];
    }
    return id;
}
