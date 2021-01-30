/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

#include "NoQueuePacketCommunication.h"


const uint8_t NoQueuePacketCommunication::DefaultMaxReceivingFailures = 3;


NoQueuePacketCommunication::NoQueuePacketCommunication(ITransceiver* lowLevelComm, uint8_t maxReceivingFailures)
    : PacketCommunication(lowLevelComm),
    MaxReceivingFailures(maxReceivingFailures),
    sendingBuffer(0)
{
}


NoQueuePacketCommunication::~NoQueuePacketCommunication()
{
}


bool NoQueuePacketCommunication::sendDataPacket(const IDataPacket* packetToSend)
{
    sendingBuffer.ensureCapacity(packetToSend->getPacketSize() + 1); // Data + PacketID
    updateBufferFromDataPacket(sendingBuffer, packetToSend);
    return LowLevelComm->send(sendingBuffer);
}


void NoQueuePacketCommunication::receiveAndUpdatePackets()
{
    bool someDataReceivedFlag = false;
    uint8_t failureCounter = 0;

    while (LowLevelComm->available() && failureCounter <= MaxReceivingFailures)
    {
        const DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
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

        updateDataPacketFromBuffer(destinationDataPacket, receivedBuffer); // TODO: maybe use result of this method in some way
        callPacketReceivedEvent(destinationDataPacket);

        someDataReceivedFlag = true;
    }

    updateConnectionStability((uint8_t)someDataReceivedFlag * 100); // 100 when true, 0 when false
}
