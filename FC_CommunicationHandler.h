// FC_CommunicationHandler.h
/*
    Created:	10/04/2020
    Author:     Jan Wielgus
*/


#ifndef _FC_COMMUNICATIONHANDLER_h
#define _FC_COMMUNICATIONHANDLER_h

#include "arduino.h"
#include <ITransferable.h>
#include <FC_Communication_Base.h>
#include <FC_GrowingArray.h>
#include <FC_SinkingQueue.h>
#include <FC_Task.h>


class FC_CommunicationHandler : public FC_Task
{
private:
    typedef FC_Communication_Base::dataPacket dataBufferType;
    typedef FC_SinkingQueue<dataBufferType> dataPacketQueue;

    struct receiveDataPacketBundle
    {
        ITransferable* packetPtr; // created outside, just a pointer to it
        dataPacketQueue* queuePtr; // dynamically allocated inside, buffer copy queue for this packet type
    };

    FC_Communication_Base comBase; // low-level communication instance
    FC_GrowingArray<receiveDataPacketBundle> receiveDataPacketsPointersArray; // array of packets to put received data (added at startup)

public:
    FC_CommunicationHandler(Stream* serial, uint8_t bufSize = 255);
        // serial - serial port. For example Serial
        // bufSize - limit of the biggest packet size in uint8_t
    ~FC_CommunicationHandler();
    bool addRaceiveDataPacketPointer(ITransferable* recDPptr, uint8_t queuedPacketsAmount = 3); // add pointer to packet where received data will be stored (with regard of pckt ID)
    void execute() override; // Receive all data to queue, update receive packets with oldest data from queue
    bool sendDataPacket(const ITransferable* packetToSend); // immediately sends passed data packet

private:
    bool receivePacketsToQueue(); // false - there was no data to receive
    void dequeueOldestPacketOfEachType();
};


#endif

