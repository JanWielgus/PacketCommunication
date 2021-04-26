/**
 * @file PacketCommunication.h
 * @author Jan Wielgus
 * @brief Abstract base class for all packet communication concrete classes.
 * @date 2020-08-18
 * 
 */

#ifndef PACKETCOMMUNICATION_H
#define PACKETCOMMUNICATION_H

#include "IConnectionStatus.h"
#include "ITransceiver.h"
#include "Packet.h"
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
         * @brief Configure filters automatically to give smooth changes in
         * connection stability return value (to make changes to be not too rapid
         * and not too slow).
         * @param frequency_Hz Receiving frequency (how many times per second
         * receiveAndUpdatePackets() method is called).
         */
        void adaptConnStabilityToFrequency(float frequency_Hz);

        /**
         * @brief Alternative to adaptConnectionStabilityToInterval() method.
         * Used to set filters manually. This value depend on update frequency
         * (the bigger frequency the bigger changeRate value)
         * @param changeRate [0.0 < changeRate < 1.0]
         * (close to 0 - connection stability value changes quickly,
         * close to 1 - connection stability value changes slowly)
         */
        void setConnectionStabilityChangeRate(float changeRate);

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
        virtual bool send(const Packet* packetToSend) = 0;


    protected:
        /**
         * @brief Receive all available data and automatically update previously
         * added packets (through addReceivePacket() method).
         * @return assessment of received data [0 <= receivedPercent <= 100]
         * (0 - no data received, 100 - all data received).
         */
        virtual Percentage receiveAndUpdatePackets() = 0;


    private:
        /**
         * @brief Use after each receiving to update conneciton stability value.
         * @param receivedPercent assessment of received data [0 <= receivedPercent <= 100]
         * (0 - no data received, 100 - all data received).
         */
        void updateConnectionStability(Percentage receivedPercent);

        /**
         * @brief Checks if data packet with this ID was already added
         * to the packet communication.
         * @param toCheck Pointer to the data packet to check.
         * @return true if there was already registered a data packet with the same packet ID,
         * false if this packet still can be registered.
         */
        bool checkIfIDAlreadyRegistered(Packet::PacketIDType id);
    };
}


#endif










        // /**
        //  * @brief 
        //  * @param dataPacket Data packet which data will be updated.
        //  * @param sourceDataBuffer Source of data to update data packet.
        //  * Passed data buffer should have packet ID in the buffer[0].
        //  * @return false if packet ID or size doesn't match buffer, or something else went wrong.
        //  * Returns true otherwise.
        //  */

        // /**
        //  * @brief Updates bytes in the packetToUpdate from sourceBuffer.
        //  * @param packetToUpdate Packet which internal buffer will be updated.
        //  * @param sourceBuffer Source of data to update packet.
        //  * @return // TODO: finish in Packet
        //  */
        // bool updatePacketFromBuffer(Packet* packetToUpdate, const DataBuffer& sourceBuffer);

        // /**
        //  * @brief Updates data in buffer from data in data packet.
        //  * Buffer AllocatedSize have to be at least data packet size + 1 (for packet ID)!
        //  * If AllocatedSize is sufficient, buffer size variable will be set to DataBuffer + 1 value,
        //  * first element in array will be packetID, next all data will be copied to the buffer array.
        //  * @param bufferToUpdate Buffer that will be filled with data from data packet byte pointers.
        //  * Buffer AllocatedSize have to be at least dataPacket size + 1.
        //  * @param sourceDataPacket Pointer to data packet to copy data from.
        //  * @return false if buffer AllocatedSize is too small (buffer AllocatedSize >= packetSize + 1),
        //  * or something else went wrong. Returns true otherwise.
        //  */
        // bool updateBufferFromPacket(AutoDataBuffer& bufferToUpdate, const Packet* sourcePacket);

        // /**
        //  * @brief Updates data in buffer from data in data packet.
        //  * bufferToUpdate have to has EXACTLY size of: data packet size + 1 (for packet ID)!
        //  * First element in array will be packetID, next all data will be copied to the buffer array.
        //  * @param bufferToUpdate Buffer that will be filled with data from data packet byte pointers.
        //  * Buffer size have to be at least dataPacket size + 1.
        //  * @param sourceDataPacket Pointer to data packet to copy data from.
        //  * @return false if buffer size is not equal size of data packet + 1 (for packet ID).
        //  * Returns true otherwise.
        //  */
        // bool updateBufferFromPacket(DataBuffer& bufferToUpdate, const Packet* sourcePacket);

        /**
         * @brief Search for data packet in the receiveDataPacketsPointers array
         * by packet id and size.
         * @param packetID ID of packet to be found.
         * @param packetSize Size of packet to be found.
         * @param indexOutput (optional) You can add pointer to size_t variable to get also packet index in the array.
         * If method return nullptr, indexOutput will remain untouched.
         * @return pointer to one of the data packets in receiveDataPacketsPointers array,
         * or nullptr if any matching packet was found.
         */
        // TODO: refactor this method structure during implementation
        //Packet* getReceiveDataPacket(uint8_t packetID, size_t packetSize, size_t* indexOutput = nullptr);
