/**
 * @file PingTest_example.ino
 * @author Jan Wielgus
 * @brief Test of time that takes to send data packet and get respond.
 * Upload this code on both deivces.
 * @date 2020-11-04
 * 
 */


#include <PacketCommunicationWithQueue.h>
#include <StreamComm.h>
#include <DataPacket.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <ByteTypes.h>



// Sent variables
uint32Byte startTime;

// Received variables
uint32Byte respondTime; // not used, because time on both devices is not synchronized


DataPacket pingRequestPacket(0);
DataPacket pingReplyPacket(1);


SoftwareSerial softSerial(10, 11); // RX, TX
StreamComm streamComm(&softSerial, 20);
NoQueuePacketCommunication comm(&streamComm);

SimpleTasker tasker(5);



class : public Task
{
    void execute() override
    {
        Serial.println("Sending ping request... ");
        startTime = micros();
        comm.sendDataPacket(&pingRequestPacket);
    }
} pingRequestTask;


class : public IExecutable
{
    void execute() override
    {
        respondTime = micros();
        comm.sendDataPacket(&pingReplyPacket);
    }
} pingRequestReceivedEvent;


class : public IExecutable
{
    void execute() override
    {
        float ping = (micros() - startTime) / 1000.f;
        Serial.print("\tGot replay. Ping: ");
        Serial.print(ping);
        Serial.println("ms");
    }
} pingReplyReceivedEvent;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    pingRequestPacket.addByteType(startTime);
    pingRequestPacket.setPacketReceivedEvent(pingRequestReceivedEvent);

    pingReplyPacket.addByteType(respondTime);
    pingReplyPacket.setPacketReceivedEvent(pingReplyReceivedEvent);

    tasker.addTask(&pingRequestTask, 0.5f, 0); // 0.5Hz ping request

    comm.addReceiveDataPacketPointer(&pingRequestPacket);
    comm.addReceiveDataPacketPointer(&pingReplyPacket);

    // communication instance is not added to tasker, because it have to run
    // as fast as possible (it's execute method is directly called in loop())
}


void loop()
{
    tasker.runLoop();
    comm.execute(); // receive packets as fast as possible
}

