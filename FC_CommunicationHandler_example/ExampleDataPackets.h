// ExampleDataPackets.h

#ifndef _EXAMPLEDATAPACKETS_h
#define _EXAMPLEDATAPACKETS_h

#include "arduino.h"
#include <DataPacketBase.h>


class RecDP_TestReceivePacket1 : public DataPacketBase
{
public: // All data could be private and make getters for them, or make a structure and read-only reference
	// all packet data
	// use only int8_t, uint8_t or types that inherits from ByteType abstract class
	// because only that type can be added to the byte array automatically
	// you can create a new type that inherits from ByteType class

	uint8_t var1;
	int8_t var2;
	int16Byte var3;
	uint8_t var4;
	floatByte var5;
	

public:
	RecDP_TestReceivePacket1()
		: DataPacketBase(51) // packet ID
	{
		// This order must be the same in the sending device data packet with ID 1

		add_uint8_t(var1);
		add_int8_t(var2);
		addByteType(var3);
		add_uint8_t(var4);
		addByteType(var5);
	}
};



class SenDP_TestToSendPacket1 : public DataPacketBase
{
public: // All data could be private and make getters for them, or make a structure and read-only reference
	// all packet data
	// use only int8_t, uint8_t or types that inherits from ByteType abstract class
	// because only that type can be added to the byte array automatically
	// you can create a new type that inherits from ByteType class

	uint8_t var1;
	int8_t var2;
	int16Byte var3;
	uint8_t var4;
	floatByte var5;


public:
	SenDP_TestToSendPacket1()
		: DataPacketBase(51) // packet ID 1
	{
		// This order must be the same in the receiving device data packet with ID 1

		add_uint8_t(var1);
		add_int8_t(var2);
		addByteType(var3);
		add_uint8_t(var4);
		addByteType(var5);
	}
};


#endif

