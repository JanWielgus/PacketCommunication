/**
 * @file StreamComm.h
 * @author Jan Wielgus
 * @brief Low level communication library for all classes
 * that implement Arduino Stream interface.
 * This library is based on the work of Cristopher Baker PacketSerial library.
 * https://github.com/bakercp/PacketSerial
 * @date 2020-08-18
 * 
 */

#ifndef STREAMCOMM_H
#define STREAMCOMM_H

#include <arduino.h>
#include "ITransceiver.h"
#include "DataBuffer.h"


class StreamComm : public ITransceiver
{
private:
    const uint8_t PacketMarker = 0;
    const size_t BufferSize;
    uint8_t* receiveBuffer;
    DataBuffer decodedData; // received and decoded data, returned by receive method
    size_t receiveBufferIndex = 0;
    Stream* stream;

    uint8_t* bufferWithChecksum; // buffer with send data with checksum value at the end
    uint8_t* encodeBuffer; // buffer with data after encoding


public:
    /**
     * @brief Construct a new Stream Comm object
     * 
     * @param streamPtr Pointer to object that its class implement Arduino Stream interface.
     * @param bufSize Max size of one data packet (in bytes)
     */
    StreamComm(Stream* streamPtr, size_t bufferSize);

    ~StreamComm();
    void begin() override;
    bool send(const uint8_t* buffer, size_t size) override;
    bool send(const DataBuffer& buffer) override;
    size_t available() override;
    DataBuffer receiveNextData() override;

    // by now, prevent from copying objects of this class
    // TODO: think about that
    StreamComm(const StreamComm& other) = delete;
    StreamComm& operator=(const StreamComm& other) = delete;


private:
    /**
     * @brief Check if passed buffer checksum is correct.
     * 
     * @param buffer pointer to the array of data (only data).
     * @param size size of the array with data (amount of bytes).
     * @param checksum checksum that that array should have.
     * @return true if array has the same checksum as checksum in parameter,
     * false otherwise.
     */
    bool checkChecksum(const uint8_t* buffer, size_t size, uint8_t checksum);

    /**
     * @brief Calculate the checksum for passed data buffer.
     * 
     * @param buffer pointer to the array with data (only data).
     * @param size size of the array with data.
     * @return checksum for the passed data buffer.
     */
    uint8_t calculateChecksum(const uint8_t* buffer, size_t size);


    /**
     * @brief Copy contents of one array to another.
     * 
     * @param source Pointer to the source array.
     * @param destination Pointer to the destination array.
     * @param size Amount of bytes to copy.
     */
    void copyArray(const uint8_t* source, const uint8_t* destination, size_t size);
};


#endif
