// IPacketTransceiver.h
/*
    Created:	29/05/2020
    Author:     Jan Wielgus
	
	Interface for all low-level communication libraries
*/

#ifndef _IPACKETTRANSCEIVER_h
#define _IPACKETTRANSCEIVER_h

#include "arduino.h"
#include <DataBuffer.h>


class IPacketTransceiver
{
public:
	virtual ~IPacketTransceiver() {}
	
	virtual void begin() = 0;
	virtual bool send(const uint8_t* buffer, size_t size) = 0;
	virtual size_t available() = 0;
	virtual DataBuffer receiveNextData() = 0;
};


#endif
