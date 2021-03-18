/**
 * @file ESP8266WiFiComm.cpp
 * @author Jan Wielgus
 * @date 2020-12-02
 * 
 */

#include "ESP8266WiFiComm.h"


ESP8266WiFiComm::ESP8266WiFiComm(const char* ssid, const char* password, uint16_t port, size_t maxPacketSize)
    :SSID(ssid), Password(password), Port(port), receiveBuffer(maxPacketSize)
{
}


void ESP8266WiFiComm::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, Password);
}


bool ESP8266WiFiComm::send(const uint8_t* buffer, size_t size)
{
    if (checkIfBeginnedUDP() == false && beginUDP() == false)
        return false;
    
    if (sendAlwaysToLastSender_flag)
        targetIPAddress = udp.remoteIP();

    udp.beginPacket(targetIPAddress, Port);
    udp.write(buffer, size);
    bool result = udp.endPacket();

    return result;
}


bool ESP8266WiFiComm::receiveData()
{
    if (checkIfBeginnedUDP() == false && beginUDP() == false) // FIXME: probably there should be || (or) statement
    {
        receiveBuffer.size = 0;
        return false;
    }

    int packetSize = udp.parsePacket();

    if (packetSize)
        receiveBuffer.size = udp.read(receiveBuffer.buffer, receiveBuffer.AllocatedSize);
    else
        receiveBuffer.size = 0;

    return receiveBuffer.size == 0 ? false : true;
}


DataBuffer ESP8266WiFiComm::getReceivedData()
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


inline bool ESP8266WiFiComm::checkIfBeginnedUDP() const
{
    return updBeginned_flag;
}
