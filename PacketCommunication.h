/**
 * @file PacketCommunication.h
 * @author Jan Wielgus
 * @brief Abstract base class for all packet communication concrete classes.
 * @date 2020-08-18
 */

#ifndef PACKETCOMMUNICATION_H
#define PACKETCOMMUNICATION_H

#include "IConnectionStatus.h"
#include "ITransceiver.h"
#include "Packet.h"
#include "AutoDataBuffer.h"
#include <EVAFilter.h>
#include <GrowingArray.h>


namespace PacketComm
{
    /**
     * @brief Base class for all packet based communication ways.
     * Provide lots of basic methods for copying and updating buffers and packets.
     * Derived classes have to implement sendDataPacket() method (for sending) and
     * execute() method (for receiving).
     */
    class PacketCommunication : public IConnectionStatus
    {
        EVAFilter connectionStabilityFilter;

    protected:
        ITransceiver* const LowLevelComm;
        SimpleDataStructures::GrowingArray<Packet*> registeredReceivePackets;
        AutoDataBuffer sendingBuffer;
        

    public:
        typedef uint8_t Percentage;

        /**
         * @brief Construct a new Packet Communication object.
         * @param lowLevelComm pointer to the low level communication instance.
         */
        explicit PacketCommunication(ITransceiver* lowLevelComm);
        virtual ~PacketCommunication();

        PacketCommunication(const PacketCommunication&) = delete;
        PacketCommunication& operator=(const PacketCommunication&) = delete;

        /**
         * @brief Adds pointer to the packet that may be received during communication.
         * There can be added only one receive data packet pointer of each ID.
         * @param receiveDataPacketPtr pointer to the data packet that may be received.
         * @return false if packet was not successfully added (eg. there was already added data packet with the same ID)
         */
        bool registerReceivePacket(Packet* receivePacket);

        /**
         * @brief Return conneciton stability in percents.
         * Method from IConnectionStatus interface.
         * @return connection stability in range from 0 (no connection) to 100 (uninterrupted connection).
         */
        Percentage getConnectionStability() override;

        /**
         * @brief Alternative to setConnStabilitySmoothness() method.
         * Configure filters automatically to give smooth changes in
         * connection stability return value (to make changes to be not too rapid
         * and not too slow).
         * @param frequency_Hz Receiving frequency (how many times per second
         * receiveAndUpdatePackets() method is called).
         */
        void adaptConnStabilityToFrequency(float frequency_Hz);

        /**
         * @brief Alternative to adaptConnStabilityToFrequency() method.
         * Used to set filters manually. This value depend on update frequency
         * (the bigger frequency the bigger changeRate value)
         * @param changeRate [0.0 < smoothness < 1.0]
         * (close to 0 - connection stability value changes quickly,
         * close to 1 - connection stability value changes slowly)
         */
        void setConnStabilitySmoothness(float smoothness);

        /**
         * @brief Receive all available data and automatically update previously added
         * receive data packets (added through addReceivePacket() method)
         * and calls their received callback.
         */
        void receive();

        /**
         * @brief Send data packet passed in a parameter.
         * @param packetToSend Pointer to the data packet that need to be sent.
         * @return false if data packet was not sent because of any reason.
         */
        virtual bool send(const Packet* packetToSend);


    protected:
        /**
         * @brief Receive all available data and automatically update previously
         * added packets (through addReceivePacket() method).
         * @return assessment of received data [0 <= receivedPercent <= 100]
         * (0 - no data received, 100 - all data received).
         */
        virtual Percentage receiveAndUpdatePackets();

        /**
         * @brief Search for packet in the registeredReceivePackets array by packet ID and size.
         * @param packetID ID of packet to be found.
         * @param packetSize (optional) Size of packet to be found.
         * @return  Pointer to the previously registered packet with provided ID and size,
         * or nullptr if such packet was not found.
         */
        Packet* getRegisteredReceivePacket(Packet::PacketIDType packetID, size_t packetSize = -1);

        /**
         * @brief Search for packet in the registeredReceivePackets array using data buffer
         * (checks it's ID and size) that could be updated with it.
         * This method don't modify neither buffer nor packet.
         * @param buffer Buffer for which we want to have a packet.
         * @return Pointer to the previously registered packet or nullptr if such packet was not found.
         */
        Packet* getRegisteredReceivePacket(const DataBuffer& buffer);


    private:
        /**
         * @brief Use after each receiving to update conneciton stability value.
         * @param receivedPercent assessment of received data [0 <= receivedPercent <= 100]
         * (0 - no data received, 100 - all data received).
         */
        void updateConnectionStability(Percentage receivedPercent);
    };
}


#endif
