/**
 * @file utils.cpp
 * @author Jan Wielgus
 * @date 2020-10-29
 * 
 */

#include "utils.h"


void copyUint8Array(uint8_t* destination, const uint8_t* source, size_t size)
{
    for (size_t i = 0; i < size; i++)
        destination[i] = source[i];
}
