// Author: Jan Wielgus
// Date: 30.01.2019
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



#include "FC_Communication_Base.h"

void receive();
void send();

// example variables - data to send and receive
uint8_t var1;
uint8_t var2;

const uint16_t MAX_MISSED_PACKETS = 5;
const uint8_t MAX_PACKET_SIZE = 25;
uint16_t missedPackets = 0;
bool comState = false;


FC_Communication_Base com(&Serial1, MAX_PACKET_SIZE);



void setup()
{
	Serial1.begin(9600);
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
	if (com.receiveData())
	{
		var1 = com.dpReceived.buffer[0];
		var2 = com.dpReceived.buffer[1];
		// at most MAX_PACKET_SIZE
		
		// you can also use com.dpReceived.size in loop as counter to unpack data
		
		// there are also checksum methods to check if data were received properly
		
		receivedDataFlag = true;
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
	com.dpToSend.buffer[0] = var1;
	com.dpToSend.buffer[1] = var2;
	// at most MAX_PACKET_SIZE
	
	com.dpToSend.size = 2; // IMPORTANT !
	
	// there are also checksum methods to check if data were received properly
	
	com.sendData();
}
