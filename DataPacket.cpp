/**
 * @file DataPacket.cpp
 * @author Jan Wielgus
 * @date 2020-08-06
 */

#include "DataPacket.h"
#include <string.h>

using namespace PacketComm;


DataPacket::DataPacket(PacketIDType packetID, uint8_t* payload, size_t payloadSize, Callback callback)
    : Packet(packetID, Type::DATA, callback)
{
    if (payload != nullptr)
    {
        this->payload = payload;
        this->payloadSize = payloadSize;
    }
}


size_t DataPacket::getDataOnly(uint8_t* outputBuffer) const
{
    memcpy(outputBuffer, payload, payloadSize);
    return payloadSize;
}


size_t DataPacket::getDataOnlySize() const
{
    return payloadSize;
}


void DataPacket::updateDataOnly(const uint8_t* inputBuffer)
{
    memcpy(payload, inputBuffer, payloadSize);
}
