// FC_CommunicationHandler.h
/*
    Created:	10/04/2020
    Author:     Jan Wielgus
*/


#ifndef _FC_COMMUNICATIONHANDLER_h
#define _FC_COMMUNICATIONHANDLER_h

#include "arduino.h"
#include <Interfaces/ITransferable.h>
#include <Interfaces/IPacketTransceiver.h>
#include <FC_GrowingArray.h>
#include <FC_SinkingQueue.h>
#include <FC_Task.h>
#include <FC_EVA_Filter.h>


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

    IPacketTransceiver* const commBase; // low-level communication instance
    FC_GrowingArray<receiveDataPacketBundle> receiveDataPacketsPointersArray; // array of packets to put received data (added at startup)

    FC_EVA_Filter conStabFilter = FC_EVA_Filter(0.73);

public:
    FC_CommunicationHandler(IPacketTransceiver* communicationBase);
        // serial - serial port. For example Serial
        // bufSize - limit of the biggest packet size in uint8_t
    ~FC_CommunicationHandler();
    bool addRaceiveDataPacketPointer(ITransferable* recDPptr, uint8_t queuedPacketsAmount = 3); // add pointer to packet where received data will be stored (with regard of pckt ID)
    void execute() override; // Receive all data to queue, update receive packets with oldest data from queue
    bool sendDataPacket(const ITransferable* packetToSend); // immediately sends passed data packet

    uint8_t getConnectionStability(); // 0 - connection lost, 100 - uninterrupted connection (can be between)
    void adaptConStabFilterToInterval(); // if using execute() periodically by tasker, use this after adding this to the tasker
    void setConStabFilterIntensity(float filterIntensity); // 0.1 : 0.99 (0.99 is the most smooth)

private:
    bool receivePacketsToQueue(); // false - there was no data to receive
    void dequeueOldestPacketOfEachType();

    void updateConnectionStability(bool receivedDataFlag);
};


#endif

