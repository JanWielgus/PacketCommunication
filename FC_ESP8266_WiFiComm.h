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



class FC_ESP8266_WiFiComm : public IPacketTransceiver
{
public:
	FC_ESP8266_WiFiComm();
	~FC_ESP8266_WiFiComm();
	
	// public interface methods
	bool send(uint8_t* buffer, size_t size) override;
	DataBuffer receiveNextData() override; // receive AT MOST ONE data packet. HAVE TO be called until available() return false (data packet was incomplete or no data)
	size_t available() override; // return false if there is no data or data packet is incomplete
	
private:
	// ...
};

#endif
