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

#ifdef ARDUINO
    #include <arduino.h>
#endif


class IDataPacket
{
public:
    virtual ~IDataPacket() {}

    /**
     * @brief Add next variable that this data packet consists of.
     * Adding order is crutial! In the beginning attach (add) all variables.
     * @param toAdd Variable that take part in this data packet
     * that implements IByteType interface.
     */
    virtual void addByteType(IByteType& toAdd) = 0;

    /**
     * @return ID od this data packet.
     * Packet ID is not included in getPacketSize() and getBytePointersArray() methods!
     */
    virtual uint8_t getPacketID() const = 0;

    /**
     * @return Size of this packet in bytes (amount of bytes that
     * this packet consists of).
     * Do not include packet ID.
     */
    virtual size_t getPacketSize() const = 0;

    /**
     * @return Array of pointers to all bytes that this data packet consists of.
     * Do not include packet ID.
     */
    virtual uint8_t** getBytePointersArray() = 0;

    /**
     * @return Array of pointers to all bytes that this data packet consists of.
     * Do not include packet ID.
     */
    virtual const uint8_t** getBytePointersArray() const = 0;

    /**
     * @brief packetReceivedEvent will be called just after when this data packet will be received.
     * @param packetReceivedEvent Object that implements IExecutable interface.
     */
    virtual void setPacketReceivedEvent(IExecutable& packetReceivedEvent) = 0;

    /**
     * @return Pointer to the previously set event, or nullptr if any event was set. 
     */
    virtual IExecutable* getPacketEventPtr() const = 0;
};


#endif
