// IPacketTransceiver.h
/*
    Created:	29/05/2020
    Author:     Jan Wielgus
	
	Just a dataBuffer structure
*/

#ifndef _DATABUFFER_h
#define _DATABUFFER_h


/**
 * @brief This class is used to store buffer pointer and it's used
 * size in pair.
 */
class DataBufferPointerPair
{
public:
	// public access to components for faseter use
	uint8_t* buffer;
	size_t size;
};


/**
 * @brief This class represents DataBuffer, has built-in dynamic memory allocation.
 * Allocated memory do not change later. There is additional variable to
 * store used size of the buffer. This enables zero cost buffer size changing.
 * To store just buffer pointer and size used by this buffer, use DataBufferPointerPair struct.
 * Remember to set buffer size (this is 0 by default)!
 */
class DataBuffer : public DataBufferPointerPair
{
public:
	// inherited size varibale contain amount of used bytes in the array (at most AllocatedSize)
	const size_t AllocatedSize; // size of the allocated buffer array

	DataBuffer(size_t bytesToAllocate)
		: AllocatedSize(bytesToAllocate)
	{
		buffer = new uint8_t[bytesToAllocate];
		size = 0;
	}


	DataBuffer(const DataBuffer& other)
		: AllocatedSize(other.AllocatedSize)
	{
		buffer = new uint8_t[other.AllocatedSize];
		for (size_t i = 0; i < other.size; i++) // copy only used part
			buffer[i] = other.buffer[i];
		
		size = other.size;
	}


	~DataBuffer()
	{
		delete[] buffer;
	}


	DataBuffer& operator=(const DataBuffer& other) = delete;
};


#endif

