// FC_ESP8266_WiFiComm.h
//
// Author: Jan Wielgus
// Created: 04.06.2020
//


#ifndef _FC_ESP8266_WIFICOMM_h
#define _FC_ESP8266_WIFICOMM_h

#include "arduino.h"
#include <Arduino.h>
#include <DataBuffer.h>
#include <IPacketTransceiver.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>



class FC_ESP8266_WiFiComm : public IPacketTransceiver
{
public:
	FC_ESP8266_WiFiComm(const char* ssid, const char* pass, uint16_t port, size_t maxPacketSize);
	~FC_ESP8266_WiFiComm();
	
	
	// setup methods
	bool isConnected();
	
	// config methods
	void setTargetIPAddress(IPAddress ipAddress);
	void setTargetIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
	void setTargetIPAddrAlwaysToLastSender(); // always before sending set targetIPAddress to udp.remoteIP()
	
	// public interface methods
	void begin() override;
	bool send(const uint8_t* buffer, size_t size) override;
	DataBuffer receiveNextData() override; // receive AT MOST ONE data packet. HAVE TO be called until available() return false (data packet was incomplete or no data)
	size_t available() override; // return false if there is no data or data packet is incomplete
	
	
private: // methods
	bool checkIfUdpBeginned(); // return true if udp was beginned and data could be sent, connect with wifi if not connected

private: // data
	bool udpBeginnedFlag = false; // set after successful wifi connection
	bool sendToRemoteIPFlag = true; // before every sending, set target IP to udp.remoteIP()
	
	WiFiUDP udp;
	IPAddress localIPAddress;
	IPAddress targetIPAddress; // IP address where data will be sent using send() method
	
	// initial config
	const char* SSID;
	const char* PASS;
	const uint16_t Port;
	const size_t MaxPacketSize;
	
	
	// data buffers
	DataBuffer receiveBuffer;
};

#endif


