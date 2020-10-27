/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

#include "NoQueuePacketCommunication.h"


NoQueuePacketCommunication::NoQueuePacketCommunication(ITransceiver* lowLevelComm, uint8_t maxReceivingFailures)
    : PacketCommunication(lowLevelComm), MaxReceivingFailures(maxReceivingFailures)
{
}


NoQueuePacketCommunication::~NoQueuePacketCommunication()
{
    if (sendingBuffer.buffer != nullptr)
        delete[] sendingBuffer.buffer;
}


bool NoQueuePacketCommunication::sendDataPacket(const IDataPacket* packetToSend)
{
    ensureSendingBufferCapacity(packetToSend->getPacketSize() + 1);
    updateBufferFromDataPacket(sendingBuffer, packetToSend);
    return LowLevelComm->send(sendingBuffer);
}


void NoQueuePacketCommunication::execute()
{
    receiveDataAndUpdateReceiveDataPackets();
}




void NoQueuePacketCommunication::ensureSendingBufferCapacity(size_t minimumSize)
{
    if (minimumSize <= sendingBuffer.size)
        return;

    if (sendingBuffer.buffer != nullptr)
        delete[] sendingBuffer.buffer;

    sendingBuffer.buffer = new uint8_t[minimumSize];
    sendingBuffer.size = minimumSize;
}


void NoQueuePacketCommunication::receiveDataAndUpdateReceiveDataPackets()
{
    bool someDataReceivedFlag = false;
    uint8_t failureCounter = 0;

    while (LowLevelComm->available() && failureCounter <= MaxReceivingFailures)
    {
        DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
        if (receivedBuffer.size == 0)
        {
            failureCounter++;
            continue;
        }
        
        IDataPacket* destinationDataPacket = getReceiveDataPacketPointer(receivedBuffer.buffer[0], receivedBuffer.size - 1);
        if (destinationDataPacket == nullptr) // any previously added receive data packets match received data
        {
            failureCounter++;
            continue;
        }

        updateDataPacketFromBuffer(destinationDataPacket, receivedBuffer);
        callPacketEvent(destinationDataPacket);

        someDataReceivedFlag = true;
    }

    updateConnectionStability((uint8_t)someDataReceivedFlag * 100); // 100 when true, 0 when false
}
