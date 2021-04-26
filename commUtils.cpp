/**
 * @file utils.cpp
 * @author Jan Wielgus
 * @date 2020-10-29
 * 
 */

#include "commUtils.h"


void PacketComm::Utils::copyBuffer(uint8_t* destination, const uint8_t* source, size_t size)
{
    for (size_t i = 0; i < size; i++)
        destination[i] = source[i];
}


bool PacketComm::Utils::copyBuffer(DataBuffer& destination, const DataBuffer& source)
{
    if (source.size != destination.size)
        return false;

    copyBuffer(destination.buffer, source.buffer, source.size);
    return true;
}


bool PacketComm::Utils::checkChecksum(const uint8_t* buffer, size_t size, uint8_t checksum)
{
    if (size == 0)
        return false;
    
    return calculateChecksum(buffer, size) == checksum;
}


inline uint8_t PacketComm::Utils::calculateChecksum(const uint8_t* buffer, size_t size)
{
    uint8_t checksum = buffer[0];
    for (size_t i = 1; i < size; i++)
        checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
    
    return checksum;
}
