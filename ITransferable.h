// ITransferable.h
/*
    Created:	09/04/2020
    Author:     Jan Wielgus
*/

#ifndef _ITRANSFERABLE_h
#define _ITRANSFERABLE_h

#include "arduino.h"
#include <FC_GrowingArray.h>


class ITransferable
{
public:
	virtual ~ITransferable();
	virtual uint8_t getPacketID() = 0;
	virtual uint8_t getPacketSize() = 0;
	virtual FC_GrowingArray<uint8_t*> getBytePointersArray() = 0;
};


#endif

