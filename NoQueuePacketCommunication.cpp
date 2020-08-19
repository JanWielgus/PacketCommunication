/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

#include "NoQueuePacketCommunication.h"
#include "C:\Users\janwi\Documents\Arduino\libraries\DataStructures\SimpleGrowingArray.h" // FIXME: remove full path


NoQueuePacketCommunication::NoQueuePacketCommunication(ITransceiver* lowLevelComm)
    : PacketCommunication(lowLevelComm)
{
    receiveDataPacketsPointers = new SimpleGrowingArray<IDataPacket*>();
}


NoQueuePacketCommunication::~NoQueuePacketCommunication()
{
    delete receiveDataPacketsPointers;
}


bool NoQueuePacketCommunication::addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr)
{
    if (checkIfAlreadyAdded(receiveDataPacketPtr))
        return false;
    
    receiveDataPacketsPointers->add(receiveDataPacketPtr);
    return true;
}


bool NoQueuePacketCommunication::sendDataPacket(const IDataPacket* packetToSend)
{
    // TODO: implement
}


void NoQueuePacketCommunication::execute()
{
    // TODO: implement
}



bool NoQueuePacketCommunication::checkIfAlreadyAdded(IDataPacket* toCheck)
{
    uint8_t toCheckID = toCheck->getPacketID();
    for (int i = 0; i < receiveDataPacketsPointers->getSize(); i++)
        if (receiveDataPacketsPointers[i]->getPacketID() == toCheckID)
            return true;
    
    return false;
}
