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
#include <Task.h>
#include "ITransceiver.h"
#include <EVAFilter.h>
#include "IDataPacket.h"
#include <IArray.h>
#include <GrowingArray.h>
#include "utils/utils.h"

// buffer[0] - packet ID
// buffer[1, 2, ...] - data

/**
 * @brief // TODO: fill the brief
 */
class PacketCommunication : public IConnectionStatus, public Task
{
private:
    EVAFilter connectionStabilityFilter;

protected:
    ITransceiver* const LowLevelComm;

    GrowingArray<IDataPacket*> recPktsPtrsContainer; // can be changed to different data structure (but keep the name to don't change code)
    IArray<IDataPacket*>& receiveDataPacketsPointers = recPktsPtrsContainer;

public:
    typedef uint8_t Percentage;

    /**
     * @brief Construct a new Packet Communication object.
     * @param lowLevelComm pointer to the low level communication instance.
     */
    PacketCommunication(ITransceiver* lowLevelComm);

    virtual ~PacketCommunication();

    /**
     * @brief Use after adding this object to the Tasker!
     * Configure filters automatically to give smooth changes in
     * connection stability return value
     * (to make changes to be not too fast and not too slow).
     */
    void adaptConnectionStabilityToInterval();

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
     * @brief Adds pointer to the data packet that may be received during communication.
     * There can be added only one receive data packet pointer of each ID.
     * Have to be overriden.
     * @param receiveDataPacketPtr pointer to the data packet that may be received.
     * @return false if packet was not successfully added (eg. there was already added data packet with the same ID)
     */
    bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr);

    /**
     * @brief Return conneciton stability in percents.
     * Method from IConnectionStatus interface.
     * @return connection stability in range from 0 (no connection) to 100 (uninterrupted connection).
     */
    Percentage getConnectionStability() override;



    /**
     * @brief Send data packet passed in a parameter.
     * Have to be overriden.
     * @param packetToSend Pointer to the data packet that need to be sent.
     * @return false if data packet was not sent because of any reason.
     */
    virtual bool sendDataPacket(const IDataPacket* packetToSend) = 0;

    /**
     * @brief Receive all available data and automatically update previously added
     * receive data packets (added through addReceiveDataPacketPointer() method).
     * Method overrided from Task class.
     * Have to be overriden.
     */
    virtual void execute() override = 0;



protected:
    /**
     * @brief Use after receiving (execute() method) to update conneciton stability value.
     * @param receivedPercent assessment of received data [0 <= receivedPercent <= 100]
     * (0 - no data received, 100 - all data received).
     */
    void updateConnectionStability(Percentage receivedPercent);

    /**
     * @brief Copy contents of source buffer to the destination buffer.
     * Buffers have to be allocated before using this function in both DataBuffers
     * and have the same size.
     * @param destination Destination buffer.
     * @param source Source buffer.
     * @return false if buffers are not the same size.
     */
    bool copyBufferData(DataBuffer& destination, const DataBuffer& source);

    /**
     * @brief Updates bytes in the dataPacket from dataBuffer (data buffer with packet ID)
     * @param dataPacket Data packet which data will be updated.
     * @param sourceDataBuffer Source of data to update data packet.
     * Passed data buffer should have packet ID in the buffer[0].
     * @return false if packet ID or size doesn't match buffer, or something else went wrong.
     * Returns true otherwise.
     */
    bool updateDataPacketFromBuffer(IDataPacket* dataPacket, const DataBuffer& sourceDataBuffer);

    /**
     * @brief Updates data in buffer from data in data packet.
     * Buffer AllocatedSize have to be at least data packet size + 1 (for packet ID)!
     * If AllocatedSize is sufficient, buffer size will be set to the DataBuffer + 1 quantity,
     * first element in array will be packetID, next all data will be copied to the buffer array.
     * @param bufferToUpdate Buffer that will be filled with data from data packet byte pointers.
     * Buffer AllocatedSize have to be at least dataPacket size + 1.
     * @param sourceDataPacket Pointer to data packet to copy data from.
     * @return false if buffer AllocatedSize is too small (buffer AllocatedSize >= packetSize + 1),
     * or something else went wrong. Returns true otherwise.
     */
    bool updateBufferFromDataPacket(ExtendedDataBuffer& bufferToUpdate, const IDataPacket* sourceDataPacket);

    /**
     * @brief Updates data in buffer from data in data packet.
     * bufferToUpdate have to has EXACTLY size of: data packet size + 1 (for packet ID)!
     * First element in array will be packetID, next all data will be copied to the buffer array.
     * @param bufferToUpdate Buffer that will be filled with data from data packet byte pointers.
     * Buffer size have to be at least dataPacket size + 1.
     * @param sourceDataPacket Pointer to data packet to copy data from.
     * @return false if buffer size is not equal size of data packet + 1 (for packet ID).
     * Returns true otherwise.
     */
    bool updateBufferFromDataPacket(DataBuffer& bufferToUpdate, const IDataPacket* sourceDataPacket);

    /**
     * @brief Calls packet event if exist.
     * @param dataPacket Pointer to data packet which packet event need to be called.
     */
    void callPacketEvent(IDataPacket* dataPacket);

    /**
     * @brief Search for data packet in the receiveDataPacketsPointers array
     * by packet id and size.
     * @param packetID ID of packet to be found.
     * @param packetSize Size of packet to be found (size of data, without ID).
     * @param indexOutput (optional) You can add pointer to size_t variable to get also packet index in the array.
     * If method return nullptr, indexOutput will remain untouched.
     * @return pointer to one of the data packets in receiveDataPacketsPointers array,
     * or nullptr if any matching packet was found.
     */
    IDataPacket* getReceiveDataPacketPointer(uint8_t packetID, size_t packetSize, size_t* indexOutput = nullptr);


private:
    /**
     * @brief Checks if data packet with this ID was already added
     * to the packet communication (only one data packet of each ID
     * can be added at the same time).
     * @param toCheck Pointer to the data packet to check.
     * @return true if there was already added a data packet with the same packet ID,
     * false if this packet still can be added.
     */
    bool checkIfAlreadyAdded(IDataPacket* toCheck);
};


#endif
