/**
 * @file Packet.cpp
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2021-04-26
 * 
 */

#include "Packet.h"


using namespace PacketComm;


Packet::Packet(uint16_t packetID, Callback callback)
    : PacketID(packetID), receivedCallback(callback)
{
}


Packet::~Packet()
{
}


inline uint16_t Packet::getID() const
{
    return (uint16_t)PacketID;
}


inline size_t Packet::getSize() const
{
    return PacketID.byteSize() + getDataOnlySize();
}


void Packet::setReceivedCallback(Callback callback)
{
    receivedCallback = callback;
}


inline Packet::Callback Packet::getReceivedCallback() const
{
    return receivedCallback;
}


inline void Packet::executeReceivedCallback()
{
    if (receivedCallback != nullptr)
        receivedCallback();
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


bool Packet::checkIfIDMatch(const uint8_t* buffer)
{
    for (uint8_t i = 0; i < PacketID.byteSize(); i++)
        if (buffer[i] != PacketID.byteArray()[i])
            return false;
    return true;
}
