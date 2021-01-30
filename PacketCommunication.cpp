/**
 * @file PacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-18
 * 
 */

#include "PacketCommunication.h"
#include "commUtils.h"


PacketCommunication::PacketCommunication(ITransceiver* lowLevelComm)
    : LowLevelComm(lowLevelComm)
{
}


PacketCommunication::~PacketCommunication()
{
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


bool PacketCommunication::addReceiveDataPacketPointer(IDataPacket* receiveDataPacketPtr)
{
    if (checkIfAlreadyAdded(receiveDataPacketPtr))
        return false;
    
    if (!receiveDataPacketsPointers.add(receiveDataPacketPtr))
        return false;
    return true;
}


PacketCommunication::Percentage PacketCommunication::getConnectionStability()
{
    return uint8_t(connectionStabilityFilter.getFilteredValue() + 0.5f);
}




void PacketCommunication::updateConnectionStability(Percentage receivedPercent)
{
    receivedPercent = constrain(receivedPercent, 0, 100);
    connectionStabilityFilter.update(receivedPercent);
}


bool PacketCommunication::copyBufferData(DataBuffer& destination, const DataBuffer& source)
{
    if (source.size != destination.size)
        return false;
    
    commUtils::copyUint8Array(destination.buffer, source.buffer, source.size);
    return true;
}


bool PacketCommunication::updateDataPacketFromBuffer(IDataPacket* dataPacket, const DataBuffer& sourceDataBuffer)
{
    if (dataPacket->getPacketID() != sourceDataBuffer.buffer[0] ||
        dataPacket->getPacketSize() + 1 != sourceDataBuffer.size) // buffer include packet ID, but dataPacket array don't
        return false;

    uint8_t** destinationBytePointers = dataPacket->getBytePointersArray();
    for (int i = 1; i < sourceDataBuffer.size; i++)
        *(destinationBytePointers[i - 1]) = sourceDataBuffer.buffer[i];
    
    return true;
}


bool PacketCommunication::updateBufferFromDataPacket(ExtendedDataBuffer& bufferToUpdate, const IDataPacket* sourceDataPacket)
{
    if (bufferToUpdate.AllocatedSize < sourceDataPacket->getPacketSize() + 1)
        return false;

    bufferToUpdate.size = sourceDataPacket->getPacketSize() + 1;
    bufferToUpdate.buffer[0] = sourceDataPacket->getPacketID();

    const uint8_t** packetDataPointersArray = sourceDataPacket->getBytePointersArray();
    for (int i = 1; i < bufferToUpdate.size; i++)
        bufferToUpdate.buffer[i] = *(packetDataPointersArray[i - 1]);
    
    return true;
}


bool PacketCommunication::updateBufferFromDataPacket(DataBuffer& bufferToUpdate, const IDataPacket* sourceDataPacket)
{
    if (bufferToUpdate.size != sourceDataPacket->getPacketSize() + 1 ||
        bufferToUpdate.buffer == nullptr)
        return false;

    bufferToUpdate.buffer[0] = sourceDataPacket->getPacketID();

    const uint8_t** packetDataPointersArray = sourceDataPacket->getBytePointersArray();
    for (int i = 1; i < bufferToUpdate.size; i++)
        bufferToUpdate.buffer[i] = *(packetDataPointersArray[i - 1]);

    return true;
}


void PacketCommunication::callPacketReceivedEvent(IDataPacket* dataPacket)
{
    IExecutable* packetReceivedEvent = dataPacket->getPacketReceivedEventPtr();
    if (packetReceivedEvent != nullptr)
        packetReceivedEvent->execute();
}




bool PacketCommunication::checkIfAlreadyAdded(IDataPacket* toCheck)
{
    uint8_t toCheckID = toCheck->getPacketID();
    for (int i = 0; i < receiveDataPacketsPointers.getSize(); i++)
        if (receiveDataPacketsPointers[i]->getPacketID() == toCheckID)
            return true;
    return false;
}


IDataPacket* PacketCommunication::getReceiveDataPacketPointer(uint8_t packetID, size_t packetSize, size_t* indexOutput)
{
    for (int i = 0; i < receiveDataPacketsPointers.getSize(); i++)
    {
        IDataPacket* currentlyCheckedPacket = receiveDataPacketsPointers[i];
        if (currentlyCheckedPacket->getPacketID() == packetID &&
            currentlyCheckedPacket->getPacketSize() == packetSize)
        {
            if (indexOutput != nullptr)
                *indexOutput = i;
            return currentlyCheckedPacket;
        }
    }

    return nullptr;
}
