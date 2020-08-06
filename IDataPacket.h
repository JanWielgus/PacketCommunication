/**
 * @file IDataPacket.h
 * @author Jan Wielgus
 * @brief Interface for all data packets.
 * DataPacket is class which you can use to create instances.
 * However data packet class just have to implement this interface.
 * @date 2020-08-06
 * 
 */

#ifndef IDATAPACKET_H
#define IDATAPACKET_H

#include <IByteType.h>
#include <IExecutable.h>


class IDataPacket
{
public:
    virtual ~IDataPacket() {}

    virtual void addByteType(IByteType& toAdd) = 0;
    virtual uint8_t getPacketID() const = 0;
    virtual size_t getPacketSize() const = 0;
    virtual const uint8_t** getBytePointersArray() const = 0;
    virtual void setPacketEvent(const IExecutable& packetEvent) = 0;
    virtual IExecutable* getPacketEventPtr() const = 0;
};


#endif
