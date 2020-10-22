/**
 * @file NoQueuePacketCommunication.cpp
 * @author Jan Wielgus
 * @date 2020-08-19
 * 
 */

// FIXME: REVIEW THIS FILE

#include "NoQueuePacketCommunication.h"


NoQueuePacketCommunication::NoQueuePacketCommunication(ITransceiver* lowLevelComm)
    : PacketCommunication(lowLevelComm)
{
}


NoQueuePacketCommunication::~NoQueuePacketCommunication()
{
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


// XXX: the last method to review
void NoQueuePacketCommunication::receiveDataAndUpdateReceiveDataPackets()
{
    bool someDataReceivedFlag = false;

    while (LowLevelComm->available())
    {
        DataBuffer receivedBuffer = LowLevelComm->receiveNextData();
        if (receivedBuffer.size == 0)
            continue;
        
        IDataPacket* destinationDataPacket = getReceiveDataPacketPointer(receivedBuffer.buffer[0], receivedBuffer.size - 1);
        if (destinationDataPacket == nullptr)
            continue; // any previously added receive data packets match received data

        updateDataPacketFromBuffer(destinationDataPacket, receivedBuffer);
        callPacketEvent(destinationDataPacket);

        someDataReceivedFlag = true;
    }

    updateConnectionStability(someDataReceivedFlag * 100); // 100 when true, 0 when false
}
