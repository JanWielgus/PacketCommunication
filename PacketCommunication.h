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

    /**
     * @brief Use after adding this object to the Tasker.
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

    /**
     * @brief Return conneciton stability in percents.
     * Method from IConnectionStatus interface.
     * Have to be overriden.
     * 
     * @return connection stability in range from 0% (no connection) to 100% (uninterrupted connection)
     */
    virtual uint8_t getConnectionStability() override = 0;

protected:
    /**
     * @brief Use after receiving (execute() method) to update conneciton stability value.
     * 
     * @param receivedPercent assessment of received data [0 <= receivedPercent <= 100]
     * (0 - no data received, 100 - all data received)
     */
    void updateConnectionStability(uint8_t receivedPercent);
};


#endif
