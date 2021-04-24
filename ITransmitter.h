/**
 * @file ITransmitter.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Interface for classes that enables transmitting
 * arrays of bytes (buffers).
 * @date 2021-04-10
 * 
 */

#ifndef ITRANSMITTER_H
#define ITRANSMITTER_H

#include "DataBuffer.h"
#include "AutoDataBuffer.h"


namespace PacketCommunication
{
    /**
     * @brief Interface for classes that enables transmitting
     * arrays of bytes (buffers).
     */
    class ITransmitter
    {
    public:
        virtual ~ITransmitter() {}

        /**
         * @brief Send data from the buffer with provided size.
         * @param buffer Pointer to the data buffer.
         * @param size Amount of data to send (size of the buffer).
         * @return false if something went wrong and data were not sent, true otherwise.
         */
        virtual bool send(const uint8_t* buffer, size_t size) = 0;

        /**
         * @brief Send data from provided data buffer.
         * @param buffer Data buffer with data to send.
         * @return false if something went wrong and data were not sent, true otherwise.
         */
        bool send(const DataBuffer& buffer)
        {
            return send(buffer.buffer, buffer.size);
        }

        /**
         * @brief Send data from provided data buffer.
         * This method could work faster than other send(..) methods
         * if AllocatedSize will be at least bigger by one than size of the data in buffer
         * (don't need to copy data to the bigger buffer to add checksum
         * (faster only for libraries that use checksum)).
         * @param buffer Buffer to send.
         * @return false if data was not sent for some reason (eg. buffer is nullptr or size is 0)
         */
        virtual bool send(const ExtendedDataBuffer& buffer)
        {
            return send(buffer.buffer, buffer.size);
        }
    };
}


#endif
