/**
 * @file DataBuffer.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief 
 * @date 2020-05-29
 * 
 */

#ifndef _DATABUFFER_h
#define _DATABUFFER_h

#ifdef ARDUINO
	#include "Arduino.h"
#endif


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
}


#endif

