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
    static const uint8_t PacketMarker; // definition in cpp
    const size_t MaxBufferSize;
    Stream* stream;

    // sending helper variables
    uint8_t* encodeBuffer; // buffer with data after encoding, used by sending methods

    // receiving helper variables
    uint8_t* receiveBuffer; // accumulate received bytes into array
    size_t receiveBufferIndex = 0; // amount of data in receiveBuffer
    DataBuffer decodedData; // received and decoded data


public:
    /**
     * @brief Construct a new Stream Comm object
     * 
     * @param streamPtr Pointer to object that its class implement Arduino Stream interface.
     * Initialize the stream manually outside this class.
     * @param bufSize Maximum size of one data packet in bytes (don't forget about place for packet ID).
     * Default size is 255.
     */
    StreamComm(Stream* streamPtr, size_t bufferSize = 255);
    ~StreamComm();

    StreamComm(const StreamComm& other) = delete;
    StreamComm& operator=(const StreamComm& other) = delete;

    // public interface
    void begin() override;
    bool send(const uint8_t* buffer, size_t size) override;
    bool send(const DataBufferBase& buffer) override;
    bool send(const DataBuffer& buffer) override;
    size_t available() override;
    DataBufferBase receiveNextData() override;


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
};


#endif
