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
#include <DataBuffer.h>
#include <IPacketTransceiver.h>

#include <Encoding/COBS.h>
//#include <Encoding/SLIP.h> // alternative



class FC_SerialCommBase : public IPacketTransceiver
{
public:
	const size_t BufferSize;

	FC_SerialCommBase(Stream* serial, size_t bufSize=255); // serial, bufSize - max size of one data packet
	~FC_SerialCommBase();
	
	// public interface methods
	void begin() override;
	bool send(const uint8_t* buffer, size_t size) override;
	const DataBuffer receiveNextData() override; // receive AT MOST ONE data packet. HAVE TO be called until available() return false (data packet was incomplete or no data)
	size_t available() override; // return false if there is no data or data packet is incomplete
	
private:
	uint8_t* bufWithChecksum; // buffer with send data with checksum value at the end
	uint8_t* encodeBuffer; // buffer with data after encoding


	// checksum value is placed at the buffer end (last thing in buffer)
	static bool checkChecksum(const uint8_t* buffer, size_t size); // xor'owanie
	static uint8_t calcChecksum(const uint8_t* buffer, size_t size);
	

	uint8_t* receiveBuffer;
	DataBuffer decodedData; // received and decoded data, returned by receive method
	size_t receiveBufferIndex = 0;
	uint8_t PacketMarker = 0;
	Stream* serial;
};

#endif
