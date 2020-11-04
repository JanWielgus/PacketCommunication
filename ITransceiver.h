/**
 * @file ITransceiver.h
 * @author Jan Wielgus
 * @brief Interface for all classes that can send and receive buffer of data.
 * @date 2020-08-04
 * 
 */

#ifndef ITRANSCEIVER_H
#define ITRANSCEIVER_H

#include "DataBuffer.h"

#ifdef ARDUINO
    #include <arduino.h>
#endif


/**
 * @brief Iterface (abstract base class) for classes that enables
 * sending and receiving array of uint8_t values.
 * Classes that implements this interface are used in packet based
 * communication as low-level.
 */
class ITransceiver
{
public:
    virtual ~ITransceiver() {}

    /**
     * @brief Initialize the communication.
     */
    virtual void begin() = 0;

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
    virtual bool send(const DataBuffer& buffer) = 0;

    /**
     * @brief Send data from provided data buffer.
     * This method could work faster than other send(..) methods
     * if AllocatedSize will be at least bigger by one than size of the data in buffer
     * (don't need to copy data to the bigger buffer to add checksum
     * (faster only for libraries that use checksum)).
     * @param buffer Buffer to send.
     * @return false if data was not sent for some reason (eg. buffer is nullptr or size is 0)
     */
    virtual bool send(const ExtendedDataBuffer& buffer) = 0;

    /**
     * @return amount of bytes that are waiting to receive
     * (or just positive value indicate, that there are some data to receive).
     * This don't assure that receiveNextData() method will return new packet -
     * data waiting at the input buffer may be corrupted.
     */
    virtual size_t available() = 0;

    /**
     * @return DataBuffer with received data or empty buffer if no data were received.
     * Make your own copy of data in the buffer or just use returned buffer.
     * After calling this method, some data could still wait for receiving.
     */
    virtual DataBuffer receiveNextData() = 0;
};


#endif
