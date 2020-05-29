	// Author: Jan Wielgus
// Date: 29.01.2019
//

#include <FC_SerialCommBase.h>


FC_SerialCommBase::FC_SerialCommBase(Stream* serial, size_t bufSize): BufferSize(bufSize)
{
	this->serial = serial;

	dpReceived.buffer = nullptr; // pointer to this array is set after successful receiving
	
	// allocate memory for receive and decode buffer
	receiveBuffer = new uint8_t[BufferSize];
	decodeBuffer = new uint8_t[BufferSize];
}

FC_SerialCommBase::~FC_SerialCommBase()
{
	delete [] receiveBuffer;
	delete [] decodeBuffer;
}


// send data from dpToSend filled before outside the com class
void FC_SerialCommBase::sendData()
{
	if (dpToSend.buffer==0 || dpToSend.size==0)
		return;
	
	uint8_t* encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(dpToSend.size)];
	
	size_t numEncoded = COBS::encode(dpToSend.buffer, dpToSend.size, encodeBuffer);
	
	serial->write(encodeBuffer, numEncoded);
	serial->write(PacketMarker);
	
	delete [] encodeBuffer;
}



bool FC_SerialCommBase::receiveData()
{
	while (serial->available() > 0)
	{
		uint8_t data = serial->read();
		
		if (data == PacketMarker)
		{
			size_t numDecoded = COBS::decode(receiveBuffer, receiveBufferIndex, decodeBuffer);
			
			dpReceived.buffer = decodeBuffer;
			dpReceived.size = numDecoded;
			
			// Function can not end here
			//receivedDataFlag = true;
			
			receiveBufferIndex = 0;
			
			// So end here
			return true; // Data packet was decoded
		}
		else
		{
			if ((receiveBufferIndex + 1) < BufferSize)
				receiveBuffer[receiveBufferIndex++] = data;
			else
			{
				// ERROR, buffer oberflow if we write. 
			}
		}
	}
	
	// Any complete data packet was received
	return false;
}



bool FC_SerialCommBase::checkChecksum(const uint8_t* buffer, size_t size)
{
	uint8_t checksum = calcChecksum(buffer, size);
	return checksum == buffer[size - 1]; // checksum is stored at the last place in buffer
}



uint8_t FC_SerialCommBase::calcChecksum(const uint8_t* buffer, size_t size)
{
	uint8_t checksum = buffer[0];
	for (int i = 1; i < size - 1; i++) // checksum value is the last thing in buffer
		checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
	
	return checksum;
}

