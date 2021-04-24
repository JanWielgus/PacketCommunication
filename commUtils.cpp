/**
 * @file utils.cpp
 * @author Jan Wielgus
 * @date 2020-10-29
 * 
 */

#include "commUtils.h"


using namespace PacketCommunication::Utils;


void copyBuffer(uint8_t* destination, const uint8_t* source, size_t size)
{
    for (size_t i = 0; i < size; i++)
        destination[i] = source[i];
}
