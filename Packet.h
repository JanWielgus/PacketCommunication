/**
 * @file Packet.h
 * @author Jan Wielgus
 * @brief Base class for all data packets.
 * @date 2021-04-22
 * 
 */

#ifndef PACKET_H
#define PACKET_H

#include <byteType.h>


namespace PacketComm
{
    class Packet
    {
    public:
        typedef void (*Callback)();
        typedef uint16_t PacketIDType;

    protected:
        enum class Type
        {
            DATA,
            EVENT,
            //STRING // TODO: implement string data packet
        };

    private:
        const uint16Byte PacketID;
        const Type packetType;
        Callback onReceiveCallback;


    public:
        /**
         * @param packetID Unique ID for this packet.
         * @param onReceiveCallback Optional. Void function that will be called
         * when this packet will be received.
         */
        explicit Packet(PacketIDType packetID, Type type, Callback onReceiveCallback = nullptr);
        virtual ~Packet();

        Packet(const Packet&) = delete;
        Packet& operator=(const Packet&) = delete;

        /**
         * @return ID of this packet.
         */
        PacketIDType getID() const;

        /**
         * @return Size of this packet in bytes (total amount of bytes that
         * this packet consists of including packet ID).
         */
        size_t getSize() const;

        /**
         * @brief Set action that will be triggered after receiving this packet.
         * @param callback Void function that will be called after receiving new data.
         */
        void setOnReceiveCallback(Callback callback);

        /**
         * @return Pointer to the previously set void method, or nullptr if was not set.
         */
        Callback getOnReceiveCallback() const; // TODO: is it necessary?

        /**
         * @brief Execute received callback. If callback was not set,this method takes no action.
         */
        void executeOnReceiveCallback(); // TODO: how to make that method visible only to PacketCommunication without making it a friend class?


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

        /**
         * @brief Enables to check ID of buffer (if that buffer was inside a packet,
         * what would be its ID).
         * @param buffer Buffer to chech its ID.
         */
        static PacketIDType getIDFromBuffer(const uint8_t* buffer);


    protected:
        Type getType() const;

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
