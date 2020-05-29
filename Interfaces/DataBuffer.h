// IPacketTransceiver.h
/*
    Created:	29/05/2020
    Author:     Jan Wielgus
	
	Just a dataBuffer structure
*/

#ifndef _IPACKETTRANSCEIVER_h
#define _IPACKETTRANSCEIVER_h

#include "arduino.h"


struct DataBuffer
{
	uint8_t* buffer;
	size_t size;
};


#endif

