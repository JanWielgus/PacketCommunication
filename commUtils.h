/**
 * @file commUtils.h
 * @author Jan Wielgus
 * @brief Some functions used by several classes.
 * @date 2020-10-29
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace commUtils
{
    /**
     * @brief Copy contents of one array to another.
     * @param destination Pointer to the destination array.
     * @param source Pointer to the source array.
     * @param size Amount of bytes to copy.
     */
    void copyUint8Array(uint8_t* destination, const uint8_t* source, size_t size);
}

#endif
