/**
 * @file NoQueuePacketCommunication.h
 * @author Jan Wielgus
 * @brief Concrete packet communication class that during
 * receiving automatically updates data in receive data packets.
 * (eg. when arrived two packets of the same type, only the newest
 * will be stored).
 * @date 2020-08-19
 * 
 */

#ifndef NOQUEUEPACKETCOMMUNICATION_H
#define NOQUEUEPACKETCOMMUNICATION_H

#include "PacketCommunication.h"
#include <C:\Users\janwi\Documents\Arduino\libraries\DataStructures\IArray.h> // FIXME: remove full path


class NoQueuePacketCommunication : public PacketCommunication
{
protected:
    IArray<IDataPacket*>* receiveDataPacketsPointers;


public:
    NoQueuePacketCommunication(ITransceiver* lowLevelComm);
    ~NoQueuePacketCommunication();
    virtual bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr) override;
    virtual bool sendDataPacket(const IDataPacket* packetToSend) override;
    virtual void execute() override;


protected:
    /**
     * @brief Checks if data packet with this ID was already added
     * to the packet communication (only one data packet of each ID
     * can be added at the same time)
     * 
     * @param toCheck Pointer to the data packet to check
     * @return true if there was already added a data packet with the same packet ID,
     * false if this packet still can be added.
     */
    bool checkIfAlreadyAdded(IDataPacket* toCheck);

    /**
     * @brief Search for data packet in the receiveDataPacketsPointers array
     * by packet id and size.
     * 
     * @param packetID ID of packet to be found.
     * @param packetSize Size of packet to be found.
     * @return pointer to one of the data packets in receiveDataPacketsPointers array,
     * or nullptr if any matching packet was found.
     */
    IDataPacket* getReceiveDataPacketPointer(uint8_t packetID, size_t packetSize);

    /**
     * @brief Updates bytes in the dataPacket from dataBuffer (data buffer with packet ID)
     * 
     * @param dataPacket Data packet which data will be updated.
     * @param dataBuffer Source of data to update data packet.
     * Passed data buffer should have packet ID in the buffer[0].
     */
    void updateDataInDataPacket(IDataPacket* dataPacket, DataBuffer sourceDataBuffer);

    /**
     * @brief Calls packet event if exist.
     * 
     * @param dataPacket Pointer to data packet which packet event need to be called.
     */
    void callPacketEvent(IDataPacket* dataPacket);
};


#endif
