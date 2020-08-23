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


DataBuffer PacketCommunication::createNewBufferAndAllocateMemory(size_t bufferSize)
{
    DataBuffer newBuffer;
    newBuffer.size = bufferSize;
    newBuffer.buffer = new uint8_t[bufferSize];
    return newBuffer;
}


bool PacketCommunication::copyBufferContents(const DataBuffer& source, DataBuffer& destination)
{
    if (source.size != destination.size)
        return false;
    
    size_t sizeToCopy = source.size;
    for (size_t i = 0; i < sizeToCopy; i++)
        destination.buffer[i] = source.buffer[i];
    return true;
}


bool PacketCommunication::updateDataInDataPacket(IDataPacket* dataPacket, DataBuffer sourceDataBuffer)
{
    if (dataPacket->getPacketID() != sourceDataBuffer.buffer[0] ||
        dataPacket->getPacketSize() != sourceDataBuffer.size - 1)
        return false;

    uint8_t** destinationBytePointers = dataPacket->getBytePointersArray();
    for (int i = 1; i < sourceDataBuffer.size; i++)
        *(destinationBytePointers[i - 1]) = sourceDataBuffer.buffer[i];
    
    return true;
}


bool PacketCommunication::updateBufferFromDataPacket(DataBuffer bufferToUpdate, const IDataPacket* sourceDataPacket)
{
    if (sourceDataPacket->getPacketSize() != bufferToUpdate.size - 1)
        return false;
    
    bufferToUpdate.buffer[0] = sourceDataPacket->getPacketID();

    const uint8_t** packetDataPointersArray = sourceDataPacket->getBytePointersArray();
    for (int i = 1; i < bufferToUpdate.size; i++)
        bufferToUpdate.buffer[i] = *(packetDataPointersArray[i - 1]);
    
    return true;
}


void PacketCommunication::callPacketEvent(IDataPacket* dataPacket)
{
    IExecutable* packetEvent = dataPacket->getPacketEventPtr();
    if (packetEvent != nullptr)
        packetEvent->execute();
}
