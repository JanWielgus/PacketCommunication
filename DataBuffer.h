/**
 * @file DataBuffer.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2020-05-29
 */

#ifndef _DATABUFFER_h
#define _DATABUFFER_h

#include <cstddef>


namespace PacketComm
{
	/**
	 * @brief This class is used to store buffer pointer and it's allocated size.
	 * This class is just a container: it don't make any allocation or
	 * deallocation inside! buffer is nullptr by default.
	 */
	class DataBuffer
	{
	public:
		uint8_t* buffer;
		size_t size;

		DataBuffer()
			: buffer(nullptr), size(0)
		{
		}

		DataBuffer(uint8_t* _buffer, size_t _size)
			: buffer(_buffer), size(_size)
		{
		}
		
		/**
		 * @brief Shallow comparizon. Two DataBuffers are equal if point to the same buffer
		 * (have the same pointer) and have the same size.
		 * @param other DataBuffer to compare.
		 * @return true if buffers have the same pointer and same size, otherwise false.
		 */
		bool operator==(const DataBuffer& other) const
		{
			return buffer == other.buffer && size == other.size;
		}
	};




	/**
	 * @brief This class is DataBuffer with built-in dynamic memory allocation.
	 * Allocated memory size can be changed later (using ensureAllocatedSize() or
	 * setAllocatedSize() methods). size variable store used size of the buffer
	 * (real size if inside AllocatedSize variable). This enables zero cost buffer size changing.
	 * Remember to set size variable (size is 0 by default)!
	 * Can be used as DataBuffer by using toDataBuffer() method.
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
         * is smaller than minSize.
         * @param minSize Minimum required size of the allocated buffer.
         * @param copyData_flag Flag, if true then after allocating new buffer
         * old data will be copied. Otherwise data could be lost.
         */
		void ensureAllocatedSize(size_t minSize, bool copyData_flag = true)
		{
			if (allocatedSize >= minSize)
				return;

			uint8_t* newBuffer = new uint8_t[minSize];

			if (copyData_flag)
				for (size_t i = 0; i < size; i++)
					newBuffer[i] = buffer[i];

			delete[] buffer;
			buffer = newBuffer;
			allocatedSize = minSize;
		}

		/**
		 * @brief Converts instance of AutoDataBuffer to DataBuffer class.
		 */
		DataBuffer toDataBuffer()
		{
			return *this;
		}
	};
}


#endif

