// ITransferable.h
/*
    Created:	09/04/2020
    Author:     Jan Wielgus
*/

#ifndef _ITRANSFERABLE_h
#define _ITRANSFERABLE_h

#include "arduino.h"
#include <FC_GrowingArray.h>
#include <FC_Task.h>


class ITransferable
{
public:
	virtual ~ITransferable() {}
	virtual uint8_t getPacketID() const = 0;
	virtual uint8_t getPacketSize() const = 0;
	virtual uint8_t** getBytePointersArray() = 0; // array of uint8_t*
	virtual const uint8_t** getBytePointersArray() const = 0; // array of uint8_t*
	
	// packet event
	virtual void setPacketEvent(FC_Task*) = 0; // optional event called by communication
	virtual FC_Task* getPacketEvent() = 0;
};


#endif

