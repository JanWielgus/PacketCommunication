/**
 * @file IReceiver.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Interface for classes that enables receiving
 * uint8_t arrays (buffers).
 * @date 2021-04-10
 */

#ifndef IRECEIVER_H
#define IRECEIVER_H

#include "DataBuffer.h"


namespace PacketComm
{
    /**
     * @brief Interface for classes that enables receiving
     * uint8_t arrays (buffers).
     */
    class IReceiver
    {
    public:
        virtual ~IReceiver() {}

        /**
         * @brief Receive next available data buffer. Call before getReceived() method
         * to get new data packet each time.
         * @return true if a data packet was received;
         * false otherwise (getReceived() method will return an empty buffer).
         */
        virtual bool receive() = 0;

        /**
         * @brief Returns the last data received by receive() method (call receive() method
         * before). Can be called multiple times in a row (to get the same buffer).
         * @return DataBuffer with received data or empty buffer if no data was received.
         */
        virtual const DataBuffer getReceived() = 0;
    };
}


#endif
