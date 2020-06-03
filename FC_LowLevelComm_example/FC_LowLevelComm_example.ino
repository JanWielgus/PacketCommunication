// Author: Jan Wielgus
// Date: 30.01.2019
//
// Modified to match IPacketTransceiver interface
// 03.06.2020
//



#include "FC_SerialCommBase.h" // Can be used any library, that implement IPacketTransceiver interface

void receive();
void send();

// example variables - data to send and receive
uint8_t var1;
uint8_t var2;

// Some config
const uint16_t MAX_MISSED_PACKETS = 5;
const uint8_t MAX_PACKET_SIZE = 25;
uint16_t missedPackets = 0;
bool comState = false;


// At this example I use seraiCommunication library, but can be used any that implements IPacketTransceiver interface
FC_SerialCommBase serialCommunicaiton(&Serial, MAX_PACKET_SIZE);

IPacketTransceiver& comm = serialCommunicaiton; // There can be put any class that implements IPacketTransceiver interface



void setup()
{
	Serial.begin(9600);
}


void loop()
{
	send();
	receive();
	delay(1000);
}



void receive()
{
	bool receivedDataFlag = false;
	// if packet is ready, then copy data to proper variables
	if (comm.available())
	{
		DataBuffer receivedData = comm.receiveNextData();

		// Additionally check if received data buffer in not empty
		if (receivedData.size > 0)
		{
			var1 = receivedData.buffer[0];
			var2 = receivedData.buffer[1];
			// at most MAX_PACKET_SIZE

			// you can also use com.dpReceived.size in loop as counter to unpack data

			// there are also checksum methods to check if data were received properly

			receivedDataFlag = true;
		}
	}
	
	if (receivedDataFlag)
	{
		missedPackets = 0;
		comState = true;
	}
	else
	{
		missedPackets++;
		if (missedPackets>MAX_MISSED_PACKETS)
			comState = false;
	}
}


void send()
{
	size_t toSendBufferSize = 2; // at most MAX_PACKET_SIZE
	uint8_t* toSendBuffer = new uint8_t[toSendBufferSize];
	toSendBuffer[0] = var1;
	toSendBuffer[1] = var2;
	// last index is toSendBufferSize-1
	
	comm.send(toSendBuffer, toSendBufferSize);
}
