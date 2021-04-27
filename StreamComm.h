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

#include "ITransceiver.h"
#include "DataBuffer.h"
#include "AutoDataBuffer.h"
#include "Encoding/COBS.h" // SLIP.h is alternative
#include "commUtils.h"


namespace PacketComm
{
    template <const size_t MaxBufferSize>
    class StreamComm : public ITransceiver
    {
        static const uint8_t PacketMarker;
        Stream* stream;

        // sending helper variables:
        uint8_t encodeBuffer[COBS::getEncodedBufferSize(MaxBufferSize)]; // buffer with data after encoding, used by sending methods

        // receiving helper variables:
        uint8_t receiveBuffer[MaxBufferSize]; // accumulate received bytes into array
        size_t receiveBufferIndex = 0; // amount of data in receiveBuffer
        uint8_t decodedData[MaxBufferSize]; // received and decoded data
        size_t decodedDataSize = 0;


    public:
        /**
         * @brief Ctor.
         * @param streamPtr Pointer to Arduino Stream object.
         * Initialize the stream manually outside of this class.
         */
        explicit StreamComm(Stream* stream);

        StreamComm(const StreamComm& other) = delete;
        StreamComm& operator=(const StreamComm& other) = delete;

        bool send(const uint8_t* buffer, size_t size) override;
        bool send(const AutoDataBuffer& buffer) override;
        bool receive() override;
        const DataBuffer getReceived() override;
    };



    template <const size_t MaxBufferSize>
    const uint8_t StreamComm<MaxBufferSize>::PacketMarker = 0;


    template <const size_t MaxBufferSize>
    StreamComm<MaxBufferSize>::StreamComm(Stream* stream)
    {
        this->stream = stream;
    }


    template <const size_t MaxBufferSize>
    bool StreamComm<MaxBufferSize>::send(const uint8_t* buffer, size_t size)
    {
        if (buffer == nullptr || size == 0 || size > MaxBufferSize)
            return false;

        uint8_t* bufferWithChecksum = new uint8_t[MaxBufferSize + 1];
        Utils::copyBuffer(bufferWithChecksum, buffer, size);
        bufferWithChecksum[size] = Utils::calculateChecksum(buffer, size); // add checksum after the last byte

        size_t numEncoded = COBS::encode(bufferWithChecksum, size + 1, encodeBuffer);

        stream->write(encodeBuffer, numEncoded);
        stream->write(PacketMarker);

        delete[] bufferWithChecksum;

        return true;
    }


    template <const size_t MaxBufferSize>
    bool StreamComm<MaxBufferSize>::send(const AutoDataBuffer& buffer)
    {
        if (buffer.size == 0 || buffer.size > MaxBufferSize)
            return false;
        
        if (buffer.size == buffer.AllocatedSize)
            return send(buffer.buffer, buffer.size);
        
        if (buffer.size > buffer.AllocatedSize) // in this case data packet is corrupted - memory used is bigger than allocated memory
            return false;
        
        // add checksum after the last byte
        buffer.buffer[buffer.size] = Utils::calculateChecksum(buffer.buffer, buffer.size);

        size_t numEncoded = COBS::encode(buffer.buffer, buffer.size + 1, encodeBuffer);

        stream->write(encodeBuffer, numEncoded);
        stream->write(PacketMarker);

        return true;
    }


    template <const size_t MaxBufferSize>
    bool StreamComm<MaxBufferSize>::receive()
    {
        while (stream->available() > 0)
        {
            uint8_t data = stream->read();

            if (data == PacketMarker)
            {
                decodedDataSize = COBS::decode(receiveBuffer, receiveBufferIndex, decodedData);

                receiveBufferIndex = 0; // reset index in the received data buffer

                uint8_t checksum = decodedData[decodedDataSize - 1];
                bool checksumResult = Utils::checkChecksum(decodedData, decodedDataSize - 1, checksum);
                if (checksumResult)
                    decodedDataSize--; // 'remove' checksum from the output buffer
                else
                    decodedDataSize = 0; // checksum failed, corrupted buffer

                return decodedDataSize == 0 ? false : true;
            }
            else
            {
                if (receiveBufferIndex + 1 < MaxBufferSize)
                    receiveBuffer[receiveBufferIndex++] = data;
                else
                {
                    // ERROR, buffer overflow if we write.
                    // Received buffer was too big to decode (increase MaxBufferSize).

                    // Finnish receiving this packet and reset the receiveBuffer
                    while (stream->available() && stream->read() != PacketMarker);
                    receiveBufferIndex = 0;
                    decodedDataSize = 0;
                    return false;
                }
            }
        }

        // No complete buffer was received.
        decodedDataSize = 0;
        return false;
    }


    template <const size_t MaxBufferSize>
    const DataBuffer StreamComm<MaxBufferSize>::getReceived()
    {
        return DataBuffer(decodedData, decodedDataSize);
    }
}


#endif
