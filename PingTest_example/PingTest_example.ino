/**
 * @file PingTest_example.ino
 * @author your name (you@domain.com)
 * @brief Test connection between two devices.
 * Upload this code on both devices.
 * @date 2021-06-11
 * 
 */

#include <BytePacket.h>
#include <StreamComm.h>
#include <PacketCommunication.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <byteType.h>

using namespace PacketComm;

const uint16_t MaxBufferSize = 20;
const float PingInterval_s = 2;

uint32_t pingRequestTime_us;
uint32_t pingCounter = 0;


void pingRequestReceivedCallback();
void pingReplyReceivedCallback();


class PingRequestPacket : public BytePacket
{
public:
    uint32Byte pingRequestCounter = 0;

    PingRequestPacket() : BytePacket(0, pingRequestReceivedCallback)
    {
        addByteType(pingRequestCounter);
    }
} pingRequestPacket;


class PingReplyCounter : public BytePacket
{
public:
    uint32Byte pingReplyCounter;

    PingReplyCounter() : BytePacket(1, pingReplyReceivedCallback)
    {
        addByteType(pingReplyCounter);
    }
} pingReplyPacket;



SoftwareSerial softSerial(10, 11); // RX, TX

StreamComm<MaxBufferSize> streamComm(&softSerial);
PacketCommunication comm(&streamComm);

SimpleTasker tasker(5);


class : public Task
{
    void execute() override
    {
        pingCounter++;
        Serial.print("Sending ping request ");
        Serial.print(pingCounter);
        Serial.println("...");

        pingRequestTime_us = micros();

        pingRequestPacket.pingRequestCounter = pingCounter;
        comm.send(&pingRequestPacket);
    }
} pingRequestTask;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    comm.registerReceivePacket(&pingRequestPacket);
    comm.registerReceivePacket(&pingReplyPacket);

    tasker.addTask(&pingRequestTask, 1.f / PingInterval_s);
    // communication instance is not added to tasker, because it have to run
    // as fast as possible (it's execute method is directly called in loop())
}


void loop()
{
    tasker.loop();
    comm.receive();
}




void pingRequestReceivedCallback()
{
    pingReplyPacket.pingReplyCounter = pingRequestPacket.pingRequestCounter;
    comm.send(&pingReplyPacket);
}


void pingReplyReceivedCallback()
{
    float ping = (micros() - pingRequestTime_us) / 1000.f;
    Serial.print("\tGot replay ");
    Serial.print(pingReplyPacket.pingReplyCounter);
    Serial.print(". Ping: ");
    Serial.print(ping);
    Serial.println("ms");
}


