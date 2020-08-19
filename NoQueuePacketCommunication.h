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
private:
    IArray<IDataPacket*>* receiveDataPacketsPointers;


public:
    NoQueuePacketCommunication(ITransceiver* lowLevelComm);
    ~NoQueuePacketCommunication();
    bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr) override;
    bool sendDataPacket(const IDataPacket* packetToSend) override;
    void execute() override;


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
};


#endif
