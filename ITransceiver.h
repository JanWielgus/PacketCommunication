/**
 * @file ITransceiver.h
 * @author Jan Wielgus
 * @brief Interface for all classes that can
 * send and receive uint8_t arrays (buffers).
 * Combine ITransmitter and IReceiver.
 * @date 2020-08-04
 */

#ifndef ITRANSCEIVER_H
#define ITRANSCEIVER_H

#include "DataBuffer.h"


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



    /**
     * @brief Iterface for classes that enables
     * sending and receiving array of uint8_t values.
     * Classes that implements this interface are used in
     * packet based communication as low-level comm way.
     */
    class ITransceiver : public ITransmitter, public IReceiver
    {
    public:
        virtual ~ITransceiver() {}
    };
}


#endif
