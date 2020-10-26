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
 * size in pair. size contain the size of the allocated buffer.
 */
class DataBuffer
{
public:
	// public access to components for faseter use
	uint8_t* buffer = nullptr;
	size_t size = 0;
};


/**
 * @brief This class is DataBuffer with built-in dynamic memory allocation.
 * Allocated memory size do not change later. There is additional variable to
 * store used size of the buffer. This enables zero cost buffer size changing.
 * To store just buffer pointer and size used by this buffer, use DataBuffer class.
 * Remember to set buffer size (size is 0 by default)!
 */
class ExtendedDataBuffer
{
private:
	DataBuffer dataBuffer;

public:
	uint8_t*& buffer;
	size_t& size; // contain amount of used bytes in the array (at most AllocatedSize)
	
	const size_t AllocatedSize; // size of the allocated buffer array

	explicit ExtendedDataBuffer(size_t bytesToAllocate)
		: AllocatedSize(bytesToAllocate), buffer(dataBuffer.buffer), size(dataBuffer.size)
	{
		dataBuffer.buffer = new uint8_t[bytesToAllocate];
		dataBuffer.size = 0;
	}


	ExtendedDataBuffer(const ExtendedDataBuffer& other)
		: AllocatedSize(other.AllocatedSize), buffer(dataBuffer.buffer), size(dataBuffer.size)
	{
		dataBuffer.buffer = new uint8_t[other.AllocatedSize];
		for (size_t i = 0; i < other.dataBuffer.size; i++) // copy only used part
			dataBuffer.buffer[i] = other.dataBuffer.buffer[i];
		
		dataBuffer.size = other.dataBuffer.size;
	}


	~ExtendedDataBuffer()
	{
		delete[] dataBuffer.buffer;
	}


	ExtendedDataBuffer& operator=(const ExtendedDataBuffer& other) = delete;


	/**
	 * @brief Used when need to store DataBuffer type
	 */
	DataBuffer toDataBuffer()
	{
		return dataBuffer;
	}
};


#endif

