// IPacketTransceiver.h
/*
    Created:	29/05/2020
    Author:     Jan Wielgus
	
	Just a dataBuffer structure
*/

#ifndef _DATABUFFER_h
#define _DATABUFFER_h

#include "arduino.h"


struct DataBuffer
{
	uint8_t* buffer;
	size_t size;
};


#endif

