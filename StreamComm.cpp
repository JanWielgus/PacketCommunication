/**
 * @file StreamComm.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 * 
 */

#include "StreamComm.h"

#include "Encoding/COBS.h"
//#include "Encoding/SLIP.h" // alternative


const uint8_t StreamComm::PacketMarker = 0;


StreamComm::StreamComm(Stream* streamPtr, size_t maxBufferSize)
    : MaxBufferSize(maxBufferSize), decodedData(MaxBufferSize)
{
    this->stream = streamPtr;

    encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(MaxBufferSize)]; // maximum size of encoded data
    receiveBuffer = new uint8_t[MaxBufferSize];
}


StreamComm::~StreamComm()
{
    delete[] receiveBuffer;
    delete[] encodeBuffer;
}


void StreamComm::begin()
{
}


bool StreamComm::send(const uint8_t* buffer, size_t size)
{
    if (buffer == nullptr || size == 0 || size > MaxBufferSize)
        return false;
    
    static ExtendedDataBuffer bufferWithChecksum(MaxBufferSize + 1);

    copyUint8Array(bufferWithChecksum.buffer, buffer, size);
    bufferWithChecksum.buffer[size] = calculateChecksum(buffer, size); // add checksum after the last byte

    size_t numEncoded = COBS::encode(bufferWithChecksum.buffer, size + 1, encodeBuffer);

    stream->write(encodeBuffer, numEncoded);
    stream->write(PacketMarker);

    return true;
}


bool StreamComm::send(const DataBuffer& buffer)
{
    return send(buffer.buffer, buffer.size);
}


bool StreamComm::send(const ExtendedDataBuffer& buffer)
{
    if (buffer.size == 0 || buffer.size > MaxBufferSize)
        return false;
    
    if (buffer.size == buffer.AllocatedSize)
        return send(buffer.buffer, buffer.size);
    
    if (buffer.size > buffer.AllocatedSize) // in this case data packet is corrupted - memory used is bigger than allocated memory
        return false;
    
    buffer.buffer[buffer.size] = calculateChecksum(buffer.buffer, buffer.size); // add checksum after the last byte

    size_t numEncoded = COBS::encode(buffer.buffer, buffer.size + 1, encodeBuffer);

    stream->write(encodeBuffer, numEncoded);
    stream->write(PacketMarker);
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
            return decodedData.toDataBuffer();
        }
        else
        {
            if (receiveBufferIndex + 1 < MaxBufferSize)
                receiveBuffer[receiveBufferIndex++] = data;
            else
            {
                // ERROR, buffer overflow if we write.
                // Received packet that is too big to decode (increase MaxBufferSize).
                
                // Finnish receiving this packet and reset the receiveBuffer
                receiveBufferIndex = 0;
                while (available() && stream->read() != PacketMarker);

                decodedData.size = 0;
                return decodedData.toDataBuffer();
            }
            
        }
        
    }

    // Any complete data packet was received
    decodedData.size = 0;
    return decodedData.toDataBuffer();
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




void copyUint8Array(uint8_t* destination, const uint8_t* source, size_t size)
{
    for (size_t i = 0; i < size; i++)
        destination[i] = source[i];
}

