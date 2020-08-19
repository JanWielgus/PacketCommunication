/**
 * @file StreamComm.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 * 
 */

#include "StreamComm.h"

#include "Encoding/COBS.h"
//#include "Encoding/SLIP.h" // alternative


StreamComm::StreamComm(Stream* streamPtr, size_t bufferSize)
    : BufferSize(bufferSize)
{
    this->stream = streamPtr;

    receiveBuffer = new uint8_t[BufferSize];
    decodedData.buffer = new uint8_t[BufferSize];
    bufferWithChecksum = new uint8_t[BufferSize + 1]; // checksum value at the end
    encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(BufferSize)]; // maximum size of encoded data
}


StreamComm::~StreamComm()
{
    delete[] receiveBuffer;
    delete[] decodedData.buffer;
    delete[] bufferWithChecksum;
    delete[] encodeBuffer;
}


void StreamComm::begin()
{
}


bool StreamComm::send(const uint8_t* buffer, size_t size)
{
    if (buffer == 0 || size == 0)
        return false;
    
    copyArray(buffer, bufferWithChecksum, size);
    bufferWithChecksum[size] = calculateChecksum(buffer, size); // add checksum at the end

    size_t numEncoded = COBS::encode(bufferWithChecksum, size + 1, encodeBuffer);

    stream->write(encodeBuffer, numEncoded);
    stream->write(PacketMarker);

    return true;
}


bool StreamComm::send(const DataBuffer& buffer)
{
    return send(buffer.buffer, buffer.size);
}


size_t StreamComm::available()
{
    return stream->available();
}


DataBuffer StreamComm::receiveNextData()
{
    while (available() > 0)
    {
        uint8_t data = stream->read();

        if (data == PacketMarker)
        {
            // decode data
            decodedData.size = COBS::decode(receiveBuffer, receiveBufferIndex, decodedData.buffer);

            // reset index in the received data buffer
            receiveBufferIndex = 0;

            uint8_t checksumOfThisBuffer = decodedData.buffer[decodedData.size - 1];
            bool checksumResult = checkChecksum(decodedData.buffer, decodedData.size - 1, checksumOfThisBuffer);
            if (checksumResult)
                decodedData.size--; // remove checksum from the copyArray
            else
                decodedData.size = 0; // checksum failed, received buffer size is set to 0
            
            // Return decoded data packet (with zero size if checksum failed)
            return decodedData;
        }
        else
        {
            if (receiveBufferIndex + 1 < BufferSize)
                receiveBuffer[receiveBufferIndex++] = data;
            else
            {
                // ERROR, buffer overflow if we write.
            }
            
        }
        
    }

    // Any complete data packet was received
    decodedData.size = 0;
    return decodedData;
}



bool StreamComm::checkChecksum(const uint8_t* buffer, size_t size, uint8_t checksum)
{
    if (size == 0)
        return false;
    
    return calculateChecksum(buffer, size) == checksum;
}


uint8_t StreamComm::calculateChecksum(const uint8_t* buffer, size_t size)
{
    uint8_t checksum = buffer[0];
    for (size_t i = 1; i < size; i++)
        checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
    
    return checksum;
}


void StreamComm::copyArray(const uint8_t* source, const uint8_t* destination, size_t size)
{
    for (size_t i = 0; i < size; i++)
        destination[i] = source[i];
}
