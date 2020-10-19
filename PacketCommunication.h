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

// buffer[0] - packet ID
// buffer[1, 2, ...] - data

class PacketCommunication : public IConnectionStatus, public Task
{
private:
    EVAFilter connectionStabilityFilter;

protected:
    ITransceiver* const LowLevelComm;

public:
    /**
     * @brief Construct a new Packet Communication object.
     * 
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
     * Used to set filters manually.
     * 
     * @param changeRate [0.0 < changeRate < 1.0]
     * (close to 0 - connection stability value changes quickly,
     * close to 1 - connection stability value changes slowly)
     */
    void setConnectionStabilityChangeRate(float changeRate);

    /**
     * @brief Return conneciton stability in percents.
     * Method from IConnectionStatus interface.
     * 
     * @return connection stability in range from 0% (no connection) to 100% (uninterrupted connection)
     */
    uint8_t getConnectionStability() override;

    /**
     * @brief Adds pointer to the data packet that may be received during communication.
     * There can be added only one receive data packet pointer of each ID.
     * Have to be overriden.
     * 
     * @param receiveDataPacketPtr pointer to the data packet that may be received.
     * @return false if packet was not successfully added (eg. there was already added data packet with the same ID)
     */
    virtual bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr) = 0;

    /**
     * @brief Send data packet passed in a parameter.
     * Have to be overriden.
     * 
     * @param packetToSend Pointer to the data packet that need to be sent.
     * @return false if data packet was not sent because of any reason.
     */
    virtual bool sendDataPacket(const IDataPacket* packetToSend) = 0;

    /**
     * @brief Receive all available data and automatically update previously added
     * receive data packets (added through addReceiveDataPacketPointer() method).
     * Method from Task class extension.
     * Have to be overriden.
     */
    virtual void execute() override = 0;

protected:
    /**
     * @brief Use after receiving (execute() method) to update conneciton stability value.
     * 
     * @param receivedPercent assessment of received data [0 <= receivedPercent <= 100]
     * (0 - no data received, 100 - all data received)
     */
    void updateConnectionStability(uint8_t receivedPercent);

    /**
     * @brief Create new buffer object and dynamically allocate memory for its buffer inside.
     * Don't forget to release this memory later using: delete[] thisBuffer.buffer;.
     * This method do not release the memory later!!
     * 
     * @param bufferSize Size of the created buffer.
     * @return Pointer to the created buffer and its size.
     */
    DataBuffer createNewBufferAndAllocateMemory(size_t bufferSize);

    /**
     * @brief Copy contents of one array to another.
     * @param source Pointer to the source array.
     * @param destination Pointer to the destination array.
     * @param size Amount of bytes to copy.
     */
    void copyArray(const uint8_t* source, uint8_t* destination, size_t size);

    /**
     * @brief Copy contents of source buffer to the destination buffer.
     * Buffers have to be allocated before using this function in both DataBuffers
     * and have the same size.
     * 
     * @param source Source buffer.
     * @param destination Destination buffer.
     * @return false if buffers are not the same size.
     */
    bool copyBufferContents(const DataBuffer& source, DataBuffer& destination);

    /**
     * @brief Updates bytes in the dataPacket from dataBuffer (data buffer with packet ID)
     * 
     * @param dataPacket Data packet which data will be updated.
     * @param sourceDataBuffer Source of data to update data packet.
     * Passed data buffer should have packet ID in the buffer[0].
     * @return false if packet ID or size doesn't match buffer, or something else went wrong.
     * Returns true otherwise.
     */
    bool updateDataPacketFromBuffer(IDataPacket* dataPacket, DataBuffer sourceDataBuffer);

    /**
     * @brief Updates data in buffer from data in data packet.
     * Buffer size have to be data packet size + 1 (for packet ID)!
     * 
     * @param bufferToUpdate Buffer that will be filled with data from data packet byte pointers.
     * Buffer size have to be dataPacket size + 1.
     * @param sourceDataPacket Pointer to data packet.
     * @return false if buffer size doesn't match packet size (bufferSize = packetSize + 1),
     * or something else went wrong. Returns true otherwise.
     */
    bool updateBufferFromDataPacket(DataBuffer bufferToUpdate, const IDataPacket* sourceDataPacket);

    /**
     * @brief Calls packet event if exist.
     * 
     * @param dataPacket Pointer to data packet which packet event need to be called.
     */
    void callPacketEvent(IDataPacket* dataPacket);
};


#endif
