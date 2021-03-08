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
     * @brief Receive next available data buffer. Call before getReceivedData() method
     * to get new data packet each time.
     * @return true if a data packet was available to receive;
     * false if there were no data (getReceivedData() method will return an empty buffer).
     */
    virtual bool receiveData() = 0;

    /**
     * @brief Call receiveData() method before to get the newest buffer. Can be called
     * multiple times in a row (to get the same buffer).
     * @return DataBuffer with received data or empty buffer if no data was received.
     * Make your own copy of data in the buffer or just use returned buffer.
     */
    virtual DataBuffer getReceivedData() = 0;
};


#endif
