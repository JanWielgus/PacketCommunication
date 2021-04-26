/**
 * @file Packet.h
 * @author Jan Wielgus
 * @brief Base class for all data packets.
 * @date 2021-04-22
 * 
 */

#ifndef PACKET_H
#define PACKET_H

#include <IExecutable.h> // TODO: if the rest of the library is not dependent on Tasker, try to remove that dependency
#include <byteType.h>


namespace PacketComm
{
    class Packet
    {
        const uint16Byte PacketID;
        IExecutable* packetReceivedEvent = nullptr;


    public:
        explicit Packet(uint16_t packetID);
        virtual ~Packet();

        Packet(const Packet&) = delete;
        Packet& operator=(const Packet&) = delete;


        /**
         * @return ID of this packet.
         */
        uint16_t getID() const;

        /**
         * @return Size of this packet in bytes (total amount of bytes that
         * this packet consists of including packet ID).
         */
        size_t getSize() const;

        /**
         * @brief packetReceivedEvent will be called just after when this data packet will be received.
         * @param packetReceivedEvent Object that implements IExecutable interface.
         */
        void setPacketReceivedEvent(IExecutable& packetReceivedEvent);

        /**
         * @return Pointer to the previously set event, or nullptr if any event was set. 
         */
        IExecutable* getPacketReceivedEvent() const;

        /**
         * @brief Fill the outputBuffer with packet's internal data (includes PacketID).
         * outputBuffer size have to be at least packet size.
         * @param outputBuffer Pointer to the array where data will be stored.
         * @return Size of this packet in bytes (same value that
         * getSize() method would return).
         */
        size_t getBuffer(uint8_t* outputBuffer) const;

        /**
         * @brief Update packet internal buffer with an inputBuffer (inputBuffer have to
         * contain PacketID, basically have to be exactly what getBuffer() method returned).
         * inputBuffer size have to be at least packet size.
         * @param inputBuffer Pointer to the array of data to update this packet.
         * @return false if inputBuffer dont match this packet ID. True otherwise.
         */
        bool updateBuffer(const uint8_t* inputBuffer);


    protected:
        /**
         * @brief Fill the outputBuffer with packet's internal data only
         * (data that this packet consists of excluding the PacketID).
         * @param outputBuffer Pointer to the array where data will be stored.
         * @return Amount of bytes that this packet consists of (excluding ID).
         */
        virtual size_t getDataOnly(uint8_t* outputBuffer) const = 0;

        /**
         * @return Size of this packet in bytes (total amount of bytes that
         * this packet consists of EXCLUDING packet ID).
         */
        virtual size_t getDataOnlySize() const = 0;

        /**
         * @brief Update packet internal buffer with an inputBuffer. inputBuffer don't
         * contain PacketID at all!
         * @param inputBuffer Pointer to the array of data to update this packet (without PacketID).
         */
        virtual void updateDataOnly(const uint8_t* inputBuffer) = 0;


    private:
        /**
         * @brief Check if beginning of the buffer contains ID of this packet.
         * @param buffer Buffer which beginning is expected to contain this packet ID.
         * @return true if buffer contain ID of this packet. False otherwise.
         */
        bool checkIfIDMatch(const uint8_t* buffer);
    };
}


#endif
