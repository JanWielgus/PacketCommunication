/**
 * @file AutoDataBuffer.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief 
 * @date 2021-04-24
 * 
 */

#ifndef AUTODATABUFFER_H
#define AUTODATABUFFER_H

#include "DataBuffer.h"


namespace PacketCommunication
{
    /**
	 * @brief This class is DataBuffer with built-in dynamic memory allocation.
	 * Allocated memory size can be change later (using ensureCapacity() method). There is additional variable to
	 * store used size of the buffer. This enables zero cost buffer size changing.
	 * Remember to set size variable (size is 0 by default)!
	 * If need to use instance of this class as DataBuffer, use toDataBuffer() method.
	 */
	class AutoDataBuffer : private DataBuffer
	{
		size_t allocatedSize = 0; // size of the allocated buffer array

	public:
		using DataBuffer::buffer;
		using DataBuffer::size; // amount of used bytes in the array, can be changed (at most allocatedSize)
		const size_t& AllocatedSize; // read-only public getter of the allocated size


        /**
         * @brief Ctor. Creates new buffer with size = 0 and
         * AllocatedSize = bytesToAllocate.
         * @param bytesToAllocate Size of the allocated memory.
         */
		explicit AutoDataBuffer(size_t bytesToAllocate)
			: AllocatedSize(allocatedSize)
		{
            buffer = new uint8_t[bytesToAllocate];
            allocatedSize = bytesToAllocate;
			size = 0;
		}


        /**
         * @brief Copy ctor. Allocate buffer of the same size as other and copy
         * only data indicated by size.
         * @param other Reference to AutoDataBuffer to be copied.
         */
		AutoDataBuffer(const AutoDataBuffer& other)
			: AllocatedSize(allocatedSize)
		{
			buffer = new uint8_t[other.allocatedSize];
            allocatedSize = other.allocatedSize;

			for (size_t i = 0; i < other.size; i++) // copy only used part
				buffer[i] = other.buffer[i];
			
			size = other.size;
		}


        /**
         * @brief Moving ctor.
         */
		AutoDataBuffer(AutoDataBuffer&& toMove) noexcept
			: AllocatedSize(allocatedSize)
		{
			buffer = toMove.buffer;
			size = toMove.size;
			allocatedSize = toMove.allocatedSize;

			toMove.buffer = nullptr;
			toMove.size = 0;
			toMove.allocatedSize = 0;
		}


		~AutoDataBuffer()
		{
			delete[] buffer;
		}


        /**
         * @brief Assignment operator. Deletes current allocated buffer.
         * Then allocate buffer of the same size as other and copy only
         * data indicated by size.
         * @param other Reference to AutoData buffer to be assigned.
         * @return Refernce to this object.
         */
		AutoDataBuffer& operator=(const AutoDataBuffer& other)
		{
            if (this != &other)
            {
                if (allocatedSize != other.allocatedSize)
                {
                    delete[] buffer;
                    buffer = new uint8_t[other.allocatedSize];
                    allocatedSize = other.allocatedSize;
                }

                for (size_t i = 0; i < other.size; i++)
                    buffer[i] = other.buffer[i];

                size = other.size;
            }

			return *this;
		}


        /**
         * @brief Moving assignment operator.
         */
		AutoDataBuffer& operator=(AutoDataBuffer&& toMove) noexcept
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
         * @brief Sets the exact size of the allocated memory.
         * Do not copy old buffer.
         * @param newAllocatedSize New size of the allocated memory.
         * If 0 then buffer is nullptr.
         */
        void setAllocatedSize(size_t newAllocatedSize)
        {
            if (allocatedSize == newAllocatedSize)
                return;

            delete[] buffer;

            if (newAllocatedSize == 0)
            {
                buffer = nullptr;
                size = 0;
                allocatedSize = 0;
            }
            else
            {
                buffer = new uint8_t[newAllocatedSize];
                size = size > newAllocatedSize ? 0 : size;
				allocatedSize = newAllocatedSize;
            }
        }


        /**
         * @brief Extends size of allocated array if AllocatedSize
         * is smaller than minCapacity.
         * @param minCapacity Minimum required size of the allocated buffer.
         * @param copyData Flag, if true then after allocating new buffer
         * old data will be copied. Otherwise data could be lost.
         */
		void ensureCapacity(size_t minCapacity, bool copyData = true)
		{
			if (allocatedSize >= minCapacity)
				return;

			uint8_t* newBuffer = new uint8_t[minCapacity];

			if (copyData)
				for (size_t i = 0; i < size; i++)
					newBuffer[i] = buffer[i];

			delete[] buffer;
			buffer = newBuffer;
			allocatedSize = minCapacity;
		}


		/**
		 * @brief Used when need to store DataBuffer type
		 */
		DataBuffer toDataBuffer()
		{
			return *this;
		}
	};
}


#endif
