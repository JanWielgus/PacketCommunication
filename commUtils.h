/**
 * @file commUtils.h
 * @author Jan Wielgus
 * @brief Some functions used by several classes.
 * @date 2020-10-29
 * 
 */

#ifndef COMMMUTILS_H
#define COMMMUTILS_H

#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace PacketCommunication
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
         * @brief Check if passed buffer checksum is correct.
         * @param buffer pointer to the array of data (only data).
         * @param size size of the array with data (amount of bytes).
         * @param checksum checksum that that array should have.
         * @return true if array has the same checksum as checksum in parameter,
         * false otherwise.
         */
        bool checkChecksum(const uint8_t* buffer, size_t size, uint8_t checksum);

        /**
         * @brief Calculate the checksum for passed data buffer.
         * @param buffer pointer to the array with data (only data).
         * @param size size of the array with data.
         * @return checksum for the passed data buffer.
         */
        uint8_t calculateChecksum(const uint8_t* buffer, size_t size);
    }
}


#endif
