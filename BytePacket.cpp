/**
 * @file BytePacket.cpp
 * @author Jan Wielgus
 * @date 2020-08-06
 * 
 */

#include "BytePacket.h"


using namespace PacketCommunication;


BytePacket::BytePacket(uint8_t packetID)
    : Packet(packetID)
{
}


void BytePacket::addByteType(IByteType& toAdd)
{
    byteTypeArray.add(&toAdd);
}


size_t BytePacket::getSize() const
{
    size_t packetSize = sizeof(PacketID);
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
        packetSize += byteTypeArray.get(i)->byteSize();
    
    return packetSize;
}


size_t BytePacket::getBuffer(uint8_t* outputBuffer) const
{
    size_t outputBufferIndex = 0; // packet size at the end
    
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
    {
        uint8_t* curByteArray = byteTypeArray.get(i)->byteArray();
        uint8_t curByteArraySize = byteTypeArray.get(i)->byteSize();

        for (uint8_t byteIndex = 0; byteIndex < curByteArraySize; ++byteIndex)
            outputBuffer[outputBufferIndex++] = curByteArray[byteIndex];
    }

    return outputBufferIndex;
}


void BytePacket::updateBuffer(const uint8_t* inputBuffer)
{
    size_t inputBufferIndex = 0; // packet size at the end
    
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
    {
        uint8_t* curByteArray = byteTypeArray.get(i)->byteArray();
        uint8_t curByteArraySize = byteTypeArray.get(i)->byteSize();

        for (uint8_t byteIndex = 0; byteIndex < curByteArraySize; ++byteIndex)
            curByteArray[byteIndex] = inputBuffer[inputBufferIndex++];
    }
}
