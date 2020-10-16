	// Author: Jan Wielgus
// Date: 29.01.2019
//

#include <FC_SerialCommBase.h>


FC_SerialCommBase::FC_SerialCommBase(Stream* serial, size_t bufSize): BufferSize(bufSize)
{
	this->serial = serial;
	
	// allocate memory for receive and decode buffer
	receiveBuffer = new uint8_t[BufferSize];
	decodedData.buffer = new uint8_t[BufferSize];
	
	
	// allocate memory for buffer with checksum value at the end
	bufWithChecksum = new uint8_t[BufferSize + 1];
	
	// allocate memory for buffer for endoced data
	encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(BufferSize)]; // maximum size of encoded data
}


FC_SerialCommBase::~FC_SerialCommBase()
{
	delete[] receiveBuffer;
	delete[] decodedData.buffer;
	
	delete[] bufWithChecksum;
	delete[] encodeBuffer;
}


void FC_SerialCommBase::begin()
{
}


bool FC_SerialCommBase::send(const uint8_t* buffer, size_t size)
{
	if (buffer==0 || size==0)
		return false;

	size++; // create room for the checksum
	
	// copy all data from buffer and add checksum at the end
	for (int i = 0; i < size - 1; i++)
		bufWithChecksum[i] = buffer[i];
	bufWithChecksum[size - 1] = calcChecksum(bufWithChecksum, size - 1);
	
	size_t numEncoded = COBS::encode(/*buffer*/ bufWithChecksum, size, encodeBuffer);
	
	serial->write(encodeBuffer, numEncoded);
	serial->write(PacketMarker);
	
	return true;
}



DataBuffer FC_SerialCommBase::receiveNextData()
{
	while (serial->available() > 0)
	{
		uint8_t data = serial->read();
		
		if (data == PacketMarker)
		{
			// decode data
			decodedData.size = COBS::decode(receiveBuffer, receiveBufferIndex, decodedData.buffer);

			// reset index in the received data buffer
			receiveBufferIndex = 0;


			// check the checksum
			if (checkChecksum(decodedData.buffer, decodedData.size))
				decodedData.size--; // remove checksum from the array
			else
				decodedData.size = 0; // checksum failed, received buffer is 0
			

			// Return decoded data packet (with zero size if checksum failed)
			return decodedData;
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
	decodedData.size = 0;
	return decodedData;
}


size_t FC_SerialCommBase::available()
{
	return serial->available();
}



// size - all data + 1 for checksum (assuming that checksum value is after data in buffer)
bool FC_SerialCommBase::checkChecksum(const uint8_t* buffer, size_t size)
{
	if (size == 0)
		return false;

	uint8_t checksum = calcChecksum(buffer, size - 1);

	return checksum == buffer[size - 1]; // checksum is stored at the last place in buffer
}


// size - only size of the data included in checksum
uint8_t FC_SerialCommBase::calcChecksum(const uint8_t* buffer, size_t size)
{
	uint8_t checksum = buffer[0];
	for (int i = 1; i < size; i++)
		checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
	
	return checksum;
}
