/**
 * @file ESP8266WiFiComm.cpp
 * @author Jan Wielgus
 * @date 2020-12-02
 */

#ifdef ESP8266

#include "ESP8266WiFiComm.h"

using namespace PacketComm;


ESP8266WiFiComm::ESP8266WiFiComm(uint16_t port, size_t maxPacketSize)
    : Port(port), receiveBuffer(maxPacketSize)
{
}


bool ESP8266WiFiComm::send(const uint8_t* buffer, size_t size)
{
    if (!beginnedUDP() && beginUDP() == false)
        return false;
    
    if (sendAlwaysToLastSender_flag)
        targetIPAddress = udp.remoteIP();

    udp.beginPacket(targetIPAddress, Port);
    udp.write(buffer, size);
    bool result = udp.endPacket();

    return result;
}


bool ESP8266WiFiComm::receive()
{
    if (!beginnedUDP() && beginUDP() == false)
    {
        receiveBuffer.size = 0;
        return false;
    }

    int packetSize = udp.parsePacket();

    if (packetSize)
        receiveBuffer.size = udp.read(receiveBuffer.buffer, receiveBuffer.AllocatedSize);
    else
        receiveBuffer.size = 0;

    return receiveBuffer.size > 0;
}


const DataBuffer ESP8266WiFiComm::getReceived()
{
    return receiveBuffer.toDataBuffer();
}



void ESP8266WiFiComm::setTargetIPAddress(IPAddress ipAddress)
{
    targetIPAddress = ipAddress;
    sendAlwaysToLastSender_flag = false;
}


void ESP8266WiFiComm::setTargetIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    targetIPAddress = IPAddress(first_octet, second_octet, third_octet, fourth_octet);
    sendAlwaysToLastSender_flag = false;
}


void ESP8266WiFiComm::setTargetIPAddressAlwaysToSender()
{
    sendAlwaysToLastSender_flag = true;
}


bool ESP8266WiFiComm::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}


IPAddress ESP8266WiFiComm::getLocalIP() const
{
    return WiFi.localIP();
}


bool ESP8266WiFiComm::beginUDP()
{
    if (updBeginned_flag || isConnected())
        return false;

    localIPAddress = WiFi.localIP();
    udp.begin(Port);
    updBeginned_flag = true;
    return true;
}

#endif // ESP8266
