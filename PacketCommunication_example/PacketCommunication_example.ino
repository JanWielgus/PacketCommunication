/**
 * @file PacketCommunication_example.ino
 * @author Jan Wielgus
 * @brief Example how to use data packet based sending and receiving using this.
 * You can make data packet consisting of variables from all the code (using ByteTypes library
 * instead of primitive types, but works just the same).
 * @date 2020-10-30
 * 
 */

#include <PacketCommunicationWithQueue.h>
#include <StreamComm.h>
#include <DataPacket.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <ByteTypes.h>


// Variables don't have to be in namespaces.
// They even don't have to be near each other.
// Each packet can have different size (but packet which will receive data
// in other device have to have the same data packet, but added as receive data packet).
namespace pckt1Vbls
{
    uint8Byte var1;
    int8Byte var2;
    int16Byte var3;
    uint8Byte var4;
    floatByte var5;
}

// in this example testPacket1 is received packet,
// but while testing, second device have to send it.
DataPacket testPacket1(51); // 51 is the unique packet ID


namespace pckt2Vbls
{
    uint8Byte var1 = 1;
    uint8Byte var2 = 2;
    int16Byte var3 = -1234;
    int8Byte var4 = -5;
}


// in this example testPacket2 is sent packet,
// but while testing, second device have to receive it
// (use addReceiveDataPacketPointer() method)
DataPacket testPacket2(39); // 39 is the uinque packet ID



SoftwareSerial softSerial(10, 11); // RX, TX
StreamComm streamComm(&softSerial, 20); // 20 is the max packet size in bytes (plus one byte for ID)
PacketCommunicationWithQueue comm(&streamComm, 10); // 10 is maximum queued data packets when receiving is slower than sending
SimpleTasker tasker(5);



void setupTestPacket1()
{
    using namespace pckt1Vbls;
    testPacket1.addByteType(var1);
    testPacket1.addByteType(var2);
    testPacket1.addByteType(var3);
    testPacket1.addByteType(var4);
    testPacket1.addByteType(var5);
}


void setupTestPacket2()
{
    using namespace pckt2Vbls;
    testPacket2.addByteType(var1);
    testPacket2.addByteType(var2);
    testPacket2.addByteType(var3);
    testPacket2.addByteType(var4);
}




class SendDataTask : public Task
{
    void execute() override
    {
        using namespace pckt2Vbls;

        var1 = var1 + 1;
        var2 = var2 + -2;
        var3 = var3 * -2;
        var4 = var4 + 10;

        // sending is just calling this method for a data packet we want to send.
        // sent are all variables previously added to the data packet using addByteType() method.
        comm.sendDataPacket(&testPacket2);
    }
};


class DataReceivedEvent : public IExecutable
{
    uint16_t counter = 0;

    void execute() override
    {
        using namespace pckt1Vbls;

        Serial.print(counter++);
        Serial.print(" Data: ");
        Serial.print("\t1: ");
        Serial.print(var1);
        Serial.print("\t2: ");
        Serial.print(var2);
        Serial.print("\t3: ");
        Serial.print(var3);
        Serial.print("\t4: ");
        Serial.print(var4);
        Serial.print("\t5: ");
        Serial.print(var5);
        Serial.print("\tStability: ");
        Serial.println(comm.getConnectionStability());
    }
};


SendDataTask sendDataTask;
DataReceivedEvent dataReceivedEvent;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);
    streamComm.begin();

    Serial.println("Program has just started!");

    setupTestPacket1();
    setupTestPacket2();


    // Add data packets that will be received
    // Other device should have exactly in opposite way: packets
    // that were added here should be used for sending data (and not be added as receive data packets),
    // but packets from here that are not listed below (are sent from here) should be
    // added as receive data packets in the other devide.
    comm.addReceiveDataPacketPointer(&testPacket1); // testPacket1 should be sending packet in the other device


    // Add communicaiton object to the tasker.
    // In each execution communication check if there are some data to receive
    // and update data packets added as receive data packets.
    tasker.addTask(&comm, 2.f, 0);
    comm.adaptConnectionStabilityToInterval(); // use after adding to tasker if you are going to measure connection stability


    // Add task which send data packet
    tasker.addTask(&sendDataTask, 4.f, 0); // sending set two times faster than receiving, to test queueing the incoming packets


    // Data packets that are received could have an events that are called
    // just after they were updated with a new data.
    // This could be used for example to update values on the screen only where new data is available.
    // In this example received data are sent to the serial monitor through the Serial port.
    testPacket1.setPacketEvent(dataReceivedEvent);
}


void loop()
{
    tasker.runLoop();
}


