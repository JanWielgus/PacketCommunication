// IPacketTransceiver.h
/*
    Created:	29/05/2020
    Author:     Jan Wielgus
	
	Just a dataBuffer structure
*/

#ifndef _DATABUFFER_h
#define _DATABUFFER_h

#ifdef ARDUINO
	#include "Arduino.h"
#endif


/**
 * @brief This class is used to store buffer pointer and it's used
 * size. size variable contain the size of the allocated memory.
 * This class is just a container: it don't make any allocation or
 * deallocation inside! buffer is nullptr by default.
 * Remember to allocate memory before use.
 */
class DataBuffer
{
public:
	// public access to components for faseter use
	uint8_t* buffer = nullptr;
	size_t size = 0;
	
	/**
	 * @brief Shallow comparizon. Two DataBuffers are equal if
	 * point to the same buffer (have the same pointer) and
	 * have the same size.
	 * @param other DataBuffer to compare.
	 * @return true if buffers have the same pointer and same size,
	 * otherwise false.
	 */
	bool operator==(const DataBuffer& other) { return buffer == other.buffer && size == other.size; }
};


/**
 * @brief This class is DataBuffer with built-in dynamic memory allocation.
 * Allocated memory size can be change later (using ensureCapacity() method). There is additional variable to
 * store used size of the buffer. This enables zero cost buffer size changing.
 * Remember to set size variable (size is 0 by default)!
 * If need to use instance of this class as DataBuffer, use toDataBuffer() method.
 */
class ExtendedDataBuffer : DataBuffer
{
	size_t allocatedSize = 0; // size of the allocated buffer array

public:
	// size - amount of used bytes in the array (at most allocatedSize)
	const size_t& AllocatedSize; // read-only public getter of the allocated size


	explicit ExtendedDataBuffer(size_t bytesToAllocate)
		: AllocatedSize(allocatedSize)
	{
		ensureCapacity(bytesToAllocate);
		size = 0;
	}


	ExtendedDataBuffer(const ExtendedDataBuffer& other)
		: AllocatedSize(allocatedSize)
	{
		allocatedSize = other.allocatedSize;
		buffer = new uint8_t[other.allocatedSize];

		for (size_t i = 0; i < other.size; i++) // copy only used part
			buffer[i] = other.buffer[i];
		
		size = other.size;
	}


	ExtendedDataBuffer(ExtendedDataBuffer&& toMove) noexcept
		: AllocatedSize(allocatedSize)
	{
		buffer = toMove.buffer;
		size = toMove.size;
		allocatedSize = toMove.allocatedSize;

		toMove.buffer = nullptr;
		toMove.size = 0;
		toMove.allocatedSize = 0;
	}


	~ExtendedDataBuffer()
	{
		delete[] buffer;
	}


	ExtendedDataBuffer& operator=(const ExtendedDataBuffer& other)
	{
		if (this != &other)
		{
			delete[] buffer;

			allocatedSize = other.allocatedSize;
			buffer = new uint8_t[other.allocatedSize];

			for (size_t i = 0; i < other.size; i++) // copy only used part
				buffer[i] = other.buffer[i];

			size = other.size;
		}

		return *this;
	}


	ExtendedDataBuffer& operator=(ExtendedDataBuffer&& toMove) noexcept
	{
		if (this != &toMove)
		{
			delete[] buffer;

			buffer = toMove.buffer;
			size = toMove.size;
			allocatedSize = toMove.allocatedSize;

			toMove.buffer = nullptr;
			toMove.size = 0;
			toMove.allocatedSize = 0;
		}

		return *this;
	}


	/**
	 * @brief If AllocatedSize is less than minCapacity, delete buffer
	 * and allocate bigger one of minCapacity size.
	 * If buffer is too small this method don't copy old contents.
	 * If need to keep the data, use ensureCapacityCopyData() method.
	 * @param minCapacity Minimum required size of the allocated buffer.
	 */
	void ensureCapacity(size_t minCapacity)
	{
		if (allocatedSize < minCapacity)
		{
			delete[] buffer;
			buffer = new uint8_t[minCapacity];
			allocatedSize = minCapacity;
		}
	}

	/**
	 * @brief This method works like ensureCapacity() method,
	 * but if buffer is too small copy contents (0 to size indexes,
	 * so only used ones).
	 * @param minCapacity Minimum required size of the allocated buffer.
	 */
	void ensureCapacityCopyData(size_t minCapacity)
	{
		if (allocatedSize < minCapacity)
		{
			uint8_t* newBuffer = new uint8_t[minCapacity];

			for (size_t i = 0; i < size; i++)
				newBuffer[i] = buffer[i];

			delete[] buffer;
			buffer = newBuffer;
			allocatedSize = minCapacity;
		}
	}


	/**
	 * @brief Used when need to store DataBuffer type
	 */
	DataBuffer toDataBuffer()
	{
		return *this;
	}
};


#endif

