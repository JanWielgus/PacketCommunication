/**
 * @file PacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 * 
 */

#include "PacketCommunication.h"
#include "commUtils.h"

using namespace PacketComm;


PacketCommunication::PacketCommunication(ITransceiver* lowLevelComm)
    : LowLevelComm(lowLevelComm),
      sendingBuffer(0)
{
}


PacketCommunication::~PacketCommunication()
{
}


bool PacketCommunication::registerReceivePacket(Packet* receivePacket)
{
    if (getRegisterdReceivePacket(receivePacket->getID()) != nullptr)
        return false;

    return registeredReceivePackets.add(receivePacket);
}


PacketCommunication::Percentage PacketCommunication::getConnectionStability()
{
    return uint8_t(connectionStabilityFilter.getFilteredValue() + 0.5f);
}


void PacketCommunication::adaptConnStabilityToFrequency(float frequency_Hz)
{
    // Thanks to this, filtering is not dependent of receiving time interval
    // Just a linear funciton that for 20000 interval return 0.85 and for 500000 return 0.5 and so on
    // Calculated using reglinp function in Excel for this two points
    float interval_us = 1000000.f / frequency_Hz;
    float filterBeta = -7.3e-7 * (float)interval_us + 0.86f;
    filterBeta = constrain(filterBeta, 0.1f, 0.97f);
    connectionStabilityFilter.setFilterBeta(filterBeta);
}


void PacketCommunication::setConnectionStabilityChangeRate(float changeRate)
{
    changeRate = constrain(changeRate, 0.0f, 0.995f);
    connectionStabilityFilter.setFilterBeta(changeRate);
}


void PacketCommunication::receive()
{
    Percentage receivingResult = receiveAndUpdatePackets();
    updateConnectionStability(receivingResult);
}


bool PacketCommunication::send(const Packet* packetToSend)
{
    // Make buffer packetSize + 1 (low level comm don't have to make separate bigger buffer to add checksum)
    sendingBuffer.ensureCapacity(packetToSend->getSize() + 1, false);
    packetToSend->getBuffer(sendingBuffer.buffer);
    return LowLevelComm->send(sendingBuffer);
}


PacketCommunication::Percentage PacketCommunication::receiveAndUpdatePackets()
{
    uint16_t receivedPacketsTotal = 0;
    uint16_t successfullyReceivedPackets = 0;

    while (LowLevelComm->receive())
    {
        const DataBuffer receivedBuffer = LowLevelComm->getReceived();
        receivedPacketsTotal++;

        Packet::PacketIDType idFromBuffer = Packet::getIDFromBuffer(receivedBuffer.buffer);
        Packet* destinationPacket = getRegisterdReceivePacket(idFromBuffer);

        if (destinationPacket == nullptr || destinationPacket->getSize() != receivedBuffer.size)
            continue;

        destinationPacket->updateBuffer(receivedBuffer.buffer);
        destinationPacket->executeReceivedCallback();

        successfullyReceivedPackets++;
    }

    // Assess receiving
    if (receivedPacketsTotal != 0)
        return ((float)successfullyReceivedPackets / receivedPacketsTotal) * 100.f;
    return 0;
}


Packet* PacketCommunication::getRegisterdReceivePacket(Packet::PacketIDType packetID)
{
    for (size_t i = 0; i < registeredReceivePackets.size(); ++i)
    {
        Packet* curPacket = registeredReceivePackets[i];
        if (curPacket->getID() == packetID)
            return curPacket;
    }

    return nullptr;
}



void PacketCommunication::updateConnectionStability(Percentage receivedPercent)
{
    // Constrain the value and update filter
    connectionStabilityFilter.update(receivedPercent > 100 ? 100 : receivedPercent);
}
