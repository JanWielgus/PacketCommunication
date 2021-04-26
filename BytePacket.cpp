/**
 * @file BytePacket.cpp
 * @author Jan Wielgus
 * @date 2020-08-06
 * 
 */

#include "BytePacket.h"


using namespace PacketComm;


BytePacket::BytePacket(uint16_t packetID)
    : Packet(packetID)
{
}


void BytePacket::addByteType(IByteType& toAdd)
{
    byteTypeArray.add(&toAdd);
}


size_t BytePacket::getDataOnly(uint8_t* outputBuffer) const
{
    size_t outputBufferIndex = 0; // at the end, data making up this packet
    
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
    {
        uint8_t* curByteArray = byteTypeArray.get(i)->byteArray();
        uint8_t curByteArraySize = byteTypeArray.get(i)->byteSize();

        for (uint8_t byteIndex = 0; byteIndex < curByteArraySize; ++byteIndex)
            outputBuffer[outputBufferIndex++] = curByteArray[byteIndex];
    }

    return outputBufferIndex;
}


size_t BytePacket::getDataOnlySize() const
{
    size_t dataOnlySize = 0;
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
        dataOnlySize += byteTypeArray.get(i)->byteSize();
    
    return dataOnlySize;
}


void BytePacket::updateDataOnly(const uint8_t* inputBuffer)
{
    size_t inputBufferIndex = 0; // at the end, data making up this packet
    
    for (size_t i = 0; i < byteTypeArray.size(); ++i)
    {
        uint8_t* curByteArray = byteTypeArray.get(i)->byteArray();
        uint8_t curByteArraySize = byteTypeArray.get(i)->byteSize();

        for (uint8_t byteIndex = 0; byteIndex < curByteArraySize; ++byteIndex)
            curByteArray[byteIndex] = inputBuffer[inputBufferIndex++];
    }
}
