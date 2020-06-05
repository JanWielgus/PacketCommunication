// Author: Jan Wielgus
// Date: 04.06.2020
//

#include <FC_ESP8266_WiFiComm.h>


FC_ESP8266_WiFiComm::FC_ESP8266_WiFiComm(const char* ssid, const char* pass, uint16_t port, size_t maxPacketSize)
	: SSID(ssid), PASS(pass), Port(port), MaxPacketSize(maxPacketSize)
{
	receiveBuffer.buffer = new uint8_t[MaxPacketSize];
}


FC_ESP8266_WiFiComm::~FC_ESP8266_WiFiComm()
{
	delete[] receiveBuffer.buffer;
}


void FC_ESP8266_WiFiComm::begin()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASS);
}


bool FC_ESP8266_WiFiComm::isConnected()
{
	return WiFi.status() == WL_CONNECTED;
}


void FC_ESP8266_WiFiComm::setTargetIPAddress(IPAddress ipAddress)
{
	targetIPAddress = ipAddress;
	sendToRemoteIPFlag = false;
}


void FC_ESP8266_WiFiComm::setTargetIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
	targetIPAddress = IPAddress(first_octet, second_octet, third_octet, fourth_octet);
	sendToRemoteIPFlag = false;
}


void FC_ESP8266_WiFiComm::setTargetIPAddrAlwaysToLastSender()
{
	sendToRemoteIPFlag = true;
}



bool FC_ESP8266_WiFiComm::send(const uint8_t* buffer, size_t size)
{
	if (checkIfUdpBeginned() == false)
		return false;
	
	if (sendToRemoteIPFlag)
		targetIPAddress = udp.remoteIP();
	
	udp.beginPacket(targetIPAddress, Port);
	udp.write(buffer, size);
	bool result = udp.endPacket();
	
	return result;
}



DataBuffer FC_ESP8266_WiFiComm::receiveNextData()
{
	if (checkIfUdpBeginned() == false)
	{
		receiveBuffer.size = 0;
		return receiveBuffer;
	}
	
	int packetSize = udp.parsePacket();
	
	if (packetSize)
	{
		receiveBuffer.size = udp.read((unsigned char *)receiveBuffer.buffer, MaxPacketSize);
		
		return receiveBuffer;
	}
	
	receiveBuffer.size = 0; // no data was received
	return receiveBuffer;
}


size_t FC_ESP8266_WiFiComm::available()
{
	//return udp.available();
	return true;
}



bool FC_ESP8266_WiFiComm::checkIfUdpBeginned()
{
	if (udpBeginnedFlag == true)
		return true;
	
	
	if (WiFi.status() == WL_CONNECTED)
	{
		localIPAddress = WiFi.localIP();
		udp.begin(Port);
		
		udpBeginnedFlag = true;
		
		return true;
	}
	else
		return false;
}

