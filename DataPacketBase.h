// DataPacketBase.h
/*
    Created:	09/04/2020
    Author:     Jan Wielgus
*/

#ifndef _DATAPACKETBASE_h
#define _DATAPACKETBASE_h

#include "arduino.h"
#include <FC_GrowingArray.h>
#include <FC_CustomDataTypes.h>
#include <Interfaces/ITransferable.h>
#include <FC_Task.h>


class DataPacketBase: public ITransferable
{
private:
    const uint8_t ID;
    FC_GrowingArray<uint8_t*> bytePointersArray;
    FC_Task* packetEvent = nullptr; // no packet event by default
    
public:
    DataPacketBase(uint8_t id);
    virtual ~DataPacketBase();

    void add_int8_t(int8_t& toAdd);
    void add_uint8_t(uint8_t& toAdd);
    void addByteType(ByteType& toAdd);

    // implementation of inherited methods from ITransferable
    uint8_t getPacketID() const override;
    uint8_t getPacketSize() const override; // ! this is also size of array below
    uint8_t** getBytePointersArray() override;  // array of uint8_t*
    const uint8_t** getBytePointersArray() const override; // array of uint8_t*
    void setPacketEvent(FC_Task* packetEvent) override; // optional event called by communication
    FC_Task* getPacketEvent() override;
};


#endif

