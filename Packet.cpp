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


inline Packet::PacketIDType Packet::getID() const
{
    return (PacketIDType)PacketID;
}


inline size_t Packet::getSize() const
{
    return PacketID.byteSize() + getDataOnlySize();
}


void Packet::setOnReceiveCallback(Callback callback)
{
    onReceiveCallback = callback;
}


inline Packet::Callback Packet::getOnReceiveCallback() const
{
    return onReceiveCallback;
}


inline void Packet::executeOnReceiveCallback()
{
    if (onReceiveCallback != nullptr)
        onReceiveCallback();
}


size_t Packet::getBuffer(uint8_t* outputBuffer) const
{
    uint8_t packetIDSize = PacketID.byteSize();
    for (uint8_t i = 0; i < packetIDSize; ++i)
        outputBuffer[i] = PacketID.byteArray()[i];
    
    return packetIDSize + getDataOnly(outputBuffer + packetIDSize);
}


bool Packet::updateBuffer(const uint8_t* inputBuffer)
{
    if (!checkIfIDMatch(inputBuffer))
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


inline Packet::Type Packet::getType() const
{
    return packetType;
}


bool Packet::checkIfIDMatch(const uint8_t* buffer)
{
    for (uint8_t i = 0; i < PacketID.byteSize(); i++)
        if (buffer[i] != PacketID.byteArray()[i])
            return false;
    return true;
}
