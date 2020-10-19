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
#include <IArray.h>


class NoQueuePacketCommunication : public PacketCommunication
{
protected:
    IArray<IDataPacket*>* receiveDataPacketsPointers;


public:
    NoQueuePacketCommunication(ITransceiver* lowLevelComm);
    virtual ~NoQueuePacketCommunication();
    virtual bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr) override;
    virtual bool sendDataPacket(const IDataPacket* packetToSend) override;
    virtual void execute() override;


private:
    void receiveDataAndUpdateReceiveDataPackets();

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
};


#endif
