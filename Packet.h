/**
 * @file Packet.h
 * @author Jan Wielgus
 * @brief Base class for all data packets.
 * @date 2020-08-06
 * 
 */

#ifndef PACKET_H
#define PACKET_H

#include <IExecutable.h> // TODO: if the rest of the library is not dependent on Tasker, try to remove that dependency


namespace PacketCommunication
{
    class Packet
    {
    protected:
        const uint8_t PacketID;
        IExecutable* packetReceivedEvent = nullptr;


    public:
        Packet(uint8_t packetID)
            : PacketID(packetID)
        {
        }

        virtual ~Packet() {}

        Packet(const Packet&) = delete;
        Packet& operator=(const Packet&) = delete;


        /**
         * @return ID od this data packet.
         */
        uint8_t getID() const
        {
            return PacketID;
        }

        /**
         * @return Size of this packet in bytes (amount of bytes that
         * this packet consists of).
         */
        virtual size_t getSize() const = 0;

        /**
         * @brief Fill the outputBuffer with packet's internal data.
         * outputBuffer size have to be at least packet size!
         * @param outputBuffer Pointer to the array where data will be stored.
         */
        virtual void getBuffer(uint8_t* outputBuffer) const = 0;

        /**
         * @brief Update internal data with a inputBuffer.
         * inputBuffer size have to be at least packet size!
         * @param inputBuffer Pointer to the array of data to update this packet.
         * @return  
         */
        virtual void updateBuffer(const uint8_t* inputBuffer) = 0;

        /**
         * @brief packetReceivedEvent will be called just after when this data packet will be received.
         * @param packetReceivedEvent Object that implements IExecutable interface.
         */
        void setPacketReceivedEvent(IExecutable& packetReceivedEvent)
        {
            this->packetReceivedEvent = &packetReceivedEvent;
        }

        /**
         * @return Pointer to the previously set event, or nullptr if any event was set. 
         */
        IExecutable* getPacketReceivedEvent() const
        {
            return packetReceivedEvent;
        }
    };
}


#endif
