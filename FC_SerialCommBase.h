// FC_Communication_Base.h
//
// Author: Jan Wielgus
// Date: 29.01.2019
//
//
//
//
// This library is based on the work of Christopher Baker
// and his PacketSerial library.
// https://github.com/bakercp/PacketSerial
//
//
//
//
//

#ifndef _FC_SERIALCOMMBASE_h
#define _FC_SERIALCOMMBASE_h

#include "arduino.h"
#include <Arduino.h>
#include <Interfaces/DataBuffer.h>
#include <Interfaces/IPacketTransceiver.h>

#include <Encoding/COBS.h>
//#include <Encoding/SLIP.h> // alternative



class FC_SerialCommBase : public IPacketTransceiver
{
public:
	const size_t BufferSize;

	FC_SerialCommBase(Stream* serial, size_t bufSize=255); // serial, bufSize - max size of one data packet
	~FC_SerialCommBase();
	
	// public interface methods
	bool send(uint8_t* buffer, size_t size) override;
	DataBuffer receiveNextData() override; // receive AT MOST ONE data packet. HAVE TO be called until available() return false (data packet was incomplete or no data)
	bool available() override; // return false if there is no data or data packet is incomplete
	
private:
	DataBuffer receivedData;

	/*
		How to use checksums:
		- buffer[0] in dataBuffer is reserved for the checksum value
		- if you are calculating the checksum, store it in the buffer[0]
		  (calculate it after packing data to dataBuffer!)
	*/
	bool checkChecksum(); // xor'owanie
	uint8_t calcChecksum();
	

	uint8_t* receiveBuffer;
	uint8_t* decodeBuffer;
	size_t receiveBufferIndex = 0;
	uint8_t PacketMarker = 0;
	Stream* serial;
};

#endif
