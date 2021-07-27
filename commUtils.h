/**
 * @file commUtils.h
 * @author Jan Wielgus
 * @brief Some functions used by several classes.
 * @date 2020-10-29
 * 
 */

#ifndef COMMMUTILS_H
#define COMMMUTILS_H

#include "DataBuffer.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace PacketComm
{
    namespace Utils
    {
        /**
         * @brief Copy contents of one array to another.
         * @param destination Pointer to the destination array.
         * @param source Pointer to the source array.
         * @param size Amount of bytes to copy.
         */
        void copyBuffer(uint8_t* destination, const uint8_t* source, size_t size);

        /**
         * @brief Copy contents of source buffer to the destination buffer.
         * Buffers have to have the same size!
         * @param destination Data will be copied to this buffer.
         * @param source Data will be copied from this buffer.
         * @return false if buffers are not the same size, true otherwise.
         */
        bool copyBuffer(DataBuffer& destination, const DataBuffer& source);

        /**
         * @brief Check if passed buffer checksum is correct.
         * @param buffer pointer to the array of data (only data).
         * @param size size of the array with data (amount of bytes).
         * @param expectedChecksum checksum that that array should have.
         * @return true if array has the same checksum as checksum in parameter,
         * false otherwise.
         */
        bool checkChecksum(const uint8_t* buffer, size_t size, uint8_t expectedChecksum);

        /**
         * @brief Calculate the checksum for passed data buffer.
         * @param buffer pointer to the array with data (only data).
         * @param size size of the array with data.
         * @return checksum for the passed data buffer.
         */
        uint8_t calculateChecksum(const uint8_t* buffer, size_t size);

        /**
         * @brief Constrains a number to be within a range.
         * @tparam T Type of constrained value
         * @return 
         */

        /**
         * @brief Constrains a number to be within a range.
         * @tparam T Type of constrained value
         * @param value Value to be constrained
         * @param min Minimum output value.
         * @param max Maximum output value.
         * @return value if it is between min and max,
         * min if value is less than min,
         * max if value is greater than max.
         */
        template <class T>
        T constrain(T value, T min, T max)
        {
            if (value < min)
                return min;
            if (value > max)
                return max;
            return value;
        }
    }
}


#endif
