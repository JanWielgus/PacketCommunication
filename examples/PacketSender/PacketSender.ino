/**
 * @file PacketSender.ino
 * @author Jan Wielgus
 * @brief Example sending program of data in form of data packets.
 * Second device have to use ExamplePacketReceiver program.
 * This are only examples but both devices could send and receive
 * data in the same time.
 * @date 2020-11-03
 * 
 */

// FIXME: update this example code


#include <PacketCommunicationWithQueue.h>
#include <StreamComm.h>
#include <DataPacket.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <ByteTypes.h>


// Type of variables that could be sent/received in a data packet
// have to extend IByteType interface class.
// Basic data types are implemented here: https://github.com/JanWielgus/ByteTypes.
// You can create your own data types that can be added to the data packet.
uint8Byte var1 = 1; // Some initial values
uint8Byte var2 = 2;
int16Byte var3 = -1234;
int8Byte var4 = -5;
// there are all int and uint types, float and special bitByte type.

// Data packet object.
// Each sent data packet have to have data packets constructed exactly
// in the same way and has the same ID as the received packet
// in the other device's code, but be added as receive data packet
// (see the example of sending).
DataPacket testPacket(51);


// In this example communication will be via SoftwareSerial.
// You can change this to any class that extend Stream class
// (and put this class into streamComm instead of softSerial).
SoftwareSerial softSerial(10, 11); // RX, TX

// This is class from this library that uses any way of communication
// that extend Stream class.
// First paramter is pointer to the class extending Stream class.
// The second one is maximum packet size in bytes + 1 (for packet ID)
// If your communication way don't extend Stream class, you can implement
// your own class that extend ITransceiver interface
// and replace streamComm in comm instance.
StreamComm streamComm(&softSerial, 20); // 20 is the max packet size in bytes (plus one byte for ID)

// This is the main communication instance. It receives pointer to class that
// extends ITransceiver interface. The second parameter is used to set the
// maximum amount of queued data packets (needed if sending frequency is faster than receiving).
// In NoQueuePacketCommunication there is no such parameter, so provide only the first one.
PacketCommunicationWithQueue comm(&streamComm, 10);

// Tasker is used to run tasks periodically.
SimpleTasker tasker(5); // 5 is maximum amount of added tasks


// You don't have to create separate functions to setup all data packets.
// Just sent and received packets have to be created in the same way
// (all byteType variables have to be added in the same order and ID have to be the same).
void setupPacket()
{
    testPacket.addByteType(var1);
    testPacket.addByteType(var2);
    testPacket.addByteType(var3);
    testPacket.addByteType(var4);
}


class SendDataTask : public Task
{
    void execute() override
    {
        // Fill data packet with some changing data
        var1 = var1 + 1;
        var2 = var2 + -2;
        var3 = 123;
        var4 = var4 + 10;

        // Sending is just calling this method for a data packet we want to send.
        // Sent are all variables previously added to the data packet using addByteType() method.
        comm.sendDataPacket(&testPacket);
    }
};


// Sending task instance
SendDataTask sendDataTask;


void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    setupPacket();

    tasker.addTask(&sendDataTask, 1.0f, 0); // 1Hz sending tasker
}


void loop()
{
    tasker.runLoop();
}

