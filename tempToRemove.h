#pragma once

#define ENABLE_TEMP

#ifdef ENABLE_TEMP

/*
#include <cstdint>
#include <cstdlib>
*/

typedef long int32_t;
typedef int int16_t;
typedef short int8_t;
typedef unsigned long uint32_t;
typedef unsigned int uint16_t;
typedef unsigned short uint8_t;
typedef uint32_t size_t;



class Stream // TODO: temp
{
public:
    bool available();
    uint8_t read();
    bool write(uint8_t*, size_t);
    bool write(uint8_t);
};

template <class T>
T constrain(T, T, T);

#endif
