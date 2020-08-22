/**
 * @file PacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 * 
 */

#include "PacketCommunication.h"


PacketCommunication::PacketCommunication(ITransceiver* lowLevelComm)
    : LowLevelComm(lowLevelComm)
{
}


PacketCommunication::~PacketCommunication()
{
}


void PacketCommunication::adaptConnectionStabilityToInterval()
{
    // Thanks to this, filtering is not dependent of receiving time interval
    // Just a linear funciton that for 20000 interval return 0.85 and for 500000 return 0.5 and so on
    // Calculated using reglinp function in Excel for this two points
    float filterBeta = constrain(-7.3e-7 * (float)getInterval() + 0.86f, 0.2f, 0.95f);
    connectionStabilityFilter.setFilterBeta(filterBeta);
}


void PacketCommunication::setConnectionStabilityChangeRate(float changeRate)
{
    changeRate = constrain(changeRate, 0.0f, 0.99f);
    connectionStabilityFilter.setFilterBeta(changeRate);
}


uint8_t PacketCommunication::getConnectionStability()
{
    return connectionStabilityFilter.getFilteredValue() + 0.5;
}


void PacketCommunication::updateConnectionStability(uint8_t receivedPercent)
{
    receivedPercent = constrain(receivedPercent, 0, 100);
    connectionStabilityFilter.update(receivedPercent);
}



