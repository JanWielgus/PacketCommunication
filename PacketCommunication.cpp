/**
 * @file PacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 */

#include "PacketCommunication.h"

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
    if (getRegisteredReceivePacket(receivePacket->getID()) != nullptr)
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


void PacketCommunication::setConnStabilitySmoothness(float smoothness)
{
    smoothness = constrain(smoothness, 0.0f, 0.995f);
    connectionStabilityFilter.setFilterBeta(smoothness);
}


void PacketCommunication::receive()
{
    Percentage receivingResult = receiveAndUpdatePackets();
    updateConnectionStability(receivingResult);
}


bool PacketCommunication::send(const Packet* packetToSend)
{
    // Make sending buffer size at least packet to send size + 1.
    // It saves time because low level comm don't have to make bigger buffer to add checksum.
    sendingBuffer.ensureAllocatedSize(packetToSend->getSize() + 1, false);
    sendingBuffer.size = packetToSend->getSize();
    packetToSend->getBuffer(sendingBuffer.buffer);
    return LowLevelComm->send(sendingBuffer);
}


PacketCommunication::Percentage PacketCommunication::receiveAndUpdatePackets()
{
    uint16_t receivedPacketsTotal = 0;
    uint16_t successfullyReceivedPackets = 0;

    while (LowLevelComm->receive())
    {
        receivedPacketsTotal++;

        const DataBuffer receivedBuffer = LowLevelComm->getReceived();

        Packet* matchingPacket = getRegisteredReceivePacket(receivedBuffer);
        if (matchingPacket == nullptr)
            continue;

        switch (matchingPacket->getType())
        {
            case Packet::Type::DATA:
                matchingPacket->updateBuffer(receivedBuffer.buffer); // this method returns bool, but should be always true
                matchingPacket->executeOnReceiveCallback();
                break;

            case Packet::Type::EVENT:
                matchingPacket->executeOnReceiveCallback();
                break;

            // other types...
            // TODO: string packet implementation

            default:
                continue; // invalid type
        }

        successfullyReceivedPackets++;
    }

    // Assess receiving
    if (receivedPacketsTotal != 0)
        return ((float)successfullyReceivedPackets / receivedPacketsTotal) * 100.f;
    return 0;
}


Packet* PacketCommunication::getRegisteredReceivePacket(Packet::PacketIDType packetID, size_t packetSize)
{
    Packet* matchingPacket = nullptr;
    for (size_t i = 0; i < registeredReceivePackets.size(); ++i)
    {
        Packet* curPacket = registeredReceivePackets[i];
        if (curPacket->getID() == packetID)
        {
            matchingPacket = curPacket;
            break;
        }
    }

    if (matchingPacket != nullptr)
        if (packetSize == matchingPacket->getSize() || packetSize == (size_t)-1)
            return matchingPacket;

    return nullptr;
}


Packet* PacketCommunication::getRegisteredReceivePacket(const DataBuffer& buffer)
{
    Packet::PacketIDType idFromBuffer = Packet::getIDFromBuffer(buffer.buffer);
    return getRegisteredReceivePacket(idFromBuffer, buffer.size);
}



void PacketCommunication::updateConnectionStability(Percentage receivedPercent)
{
    // Constrain the value and update filter
    connectionStabilityFilter.update(receivedPercent > 100 ? 100 : receivedPercent);
}
