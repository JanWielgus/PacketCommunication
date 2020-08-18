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


class PacketCommunication : public IConnectionStatus, public Task
{
protected:
    ITransceiver* const lowLevelComm;
    EVAFilter connectionStabilityFilter;

public:
    PacketCommunication(ITransceiver* lowLevelComm);
    void adaptConnectionStabilityToInterval();
    void setConnectionStabilityChangeRate(float changeRate);

    virtual bool addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr) = 0;
    virtual bool sendDataPacket(const IDataPacket* packetToSend) = 0;

protected:
    void updateConnectionStability(uint8_t receivedPercent);
};


#endif
