/**
 * @file ITransmitter.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Interface for classes that enables sending
 * uint8_t arrays (buffers).
 * @date 2021-04-10
 */

#ifndef ITRANSMITTER_H
#define ITRANSMITTER_H

#include "DataBuffer.h"
#include "AutoDataBuffer.h"


namespace PacketComm
{
    /**
     * @brief Interface for classes that enables sending
     * uint8_t arrays (buffers).
     */
    class ITransmitter
    {
    public:
        virtual ~ITransmitter() {}

        /**
         * @brief Send data from the buffer with provided size.
         * @param buffer Pointer to the data buffer.
         * @param size Amount of data to send (size of the buffer).
         * @return true if data were sent, false otherewise.
         */
        virtual bool send(const uint8_t* buffer, size_t size) = 0;

        /**
         * @brief Send data from provided data buffer.
         * @param buffer Buffer to send.
         * @return true if data were sent, false otherewise.
         */
        bool send(const DataBuffer& buffer)
        {
            return send(buffer.buffer, buffer.size);
        }

        /**
         * @brief Send data from provided data buffer.
         * This method could work faster than other send(..) methods
         * if AllocatedSize will be bigger than size of the data in buffer
         * (some low-level classes have to copy data to bigger buffers
         * in order to add some information).
         * @param buffer Buffer to send.
         * @return true if data were sent, false otherewise.
         */
        virtual bool send(const AutoDataBuffer& buffer)
        {
            return send(buffer.buffer, buffer.size);
        }
    };
}


#endif
