/**
 * @file PingTest.ino
 * @author your name (you@domain.com)
 * @brief Test connection between two devices.
 * Upload this code on both devices.
 * @date 2021-06-11
 * 
 */

#include <DataPacket.h>
#include <StreamComm.h>
#include <PacketCommunication.h>
#include <SoftwareSerial.h>
#include <byteType.h>

using namespace PacketComm;

const uint16_t MaxBufferSize = 20;
const float PingInterval_s = 2;

uint32_t pingRequestTime_us;
uint32_t pingCounter = 0;
uint32_t nextPingRequestTime_ms = 0;

void sendPingRequest();
void pingRequestReceivedCallback();
void pingReplyReceivedCallback();

SoftwareSerial softSerial(10, 11); // RX, TX
StreamComm<MaxBufferSize> streamComm(&softSerial);
PacketCommunication comm(&streamComm);


class PingRequestPacket : public DataPacket
{
public:
    uint32Byte pingRequestCounter = 0;

    PingRequestPacket()
        : DataPacket(0, pingRequestReceivedCallback)
    {
        addVar(pingRequestCounter);
    }
} pingRequestPacket;


class PingReplyPacket : public DataPacket
{
public:
    uint32Byte pingReplyCounter;

    PingReplyPacket()
        : DataPacket(1, pingReplyReceivedCallback)
    {
        addVar(pingReplyCounter);
    }
} pingReplyPacket;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    comm.registerReceivePacket(&pingRequestPacket);
    comm.registerReceivePacket(&pingReplyPacket);
}


void loop()
{
    while (millis() < nextPingRequestTime_ms) // Wait for ping reply between next ping requests
        comm.receive();
    
    sendPingRequest();
    nextPingRequestTime_ms += PingInterval_s * 1000.f;
}




void sendPingRequest() 
{
    pingCounter++;
    Serial.print("Sending ping request ");
    Serial.print(pingCounter);
    Serial.println("...");

    pingRequestTime_us = micros();

    pingRequestPacket.pingRequestCounter = pingCounter;
    comm.send(&pingRequestPacket);
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
    Serial.println("ms\n");
}


