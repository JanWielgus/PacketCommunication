/**
 * @file PacketSender.ino
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Example sending program of data in data packets.
 * Second device have to use PacketReceiver program.
 * @date 2021-07-27
 * 
 */

#include <DataPacket.h>
#include <StreamComm.h>
#include <PacketCommunication.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <byteType.h>

using namespace PacketComm;

const uint16_t MaxBufferSize = 20;


/**
 * In this example software serial is used, but you can change it
 * to other Stream (for example normal hardware serial - Serial)
 * or even change low-level communication way 
 * (see what is passed as param to comm object).
 */
SoftwareSerial softSerial(10, 11); // RX, TX
StreamComm<MaxBufferSize> streamComm(&softSerial);
PacketCommunication comm(&streamComm);

SimpleTasker tasker(5);


/**
 * Packet in receiver code have to be constructed in the same way
 * (important parts are: order in which variables are added to this packet
 * using addVar() method and packet ID).
 * You can create many data packets. Remember that they have to have different ID!
 * This packet ID is 51.
 * 
 * After ID there is optional parameter where you can set void function
 * that will be called each time this packet will be received
 * (this is skipped here because it is useless if packet is only sent).
 * At the end one instance of this class (packet) is created.
 */
class TestPacket : public DataPacket
{
public:
    uint32Byte var1 = 1; // Some initial values
    uint8Byte var2 = 2;
    int16Byte var3 = -1234;
    floatByte var4 = -5.f;

    TestPacket()
        : DataPacket(51)
    {
        addVar(var1);
        addVar(var2);
        addVar(var3);
        addVar(var4);
    }
} testPacket;


/**
 * Task that updates values of sent variables and sends whole data packet.
 */
class : public Task
{
    void execute() override
    {
        // Fill data packet with some data
        testPacket.var1 = testPacket.var1 * 2;
        testPacket.var2 = testPacket.var2 - 2;
        testPacket.var3 = 123;
        testPacket.var4 = testPacket.var4 + 2.5f;

        // Sent are all variables added at the beginning to this data packet using addVar() method.
        comm.send(&testPacket);
    }
} sendDataTask;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    tasker.addTask(&sendDataTask, 1.0f); // 1Hz sending tasker
}


void loop()
{
    tasker.loop();
}




// /**
//  * Type of variables that could be sent/received in a data packet
//  * have to extend IByteType interface class.
//  * Basic data types are implemented here: https://github.com/JanWielgus/ByteTypes.
//  * You can create your own data types that can be directly added to the data packet
//  * (by implementing IByteType interface).
//  */
// uint8Byte var1 = 1; // Some initial values
// uint8Byte var2 = 2;
// int16Byte var3 = -1234;
// int8Byte var4 = -5;
// // there are all int and uint types, float and special bitByte type.

// // Data packet object.
// // Each sent data packet have to have data packets constructed exactly
// // in the same way and has the same ID as the received packet
// // in the other device's code, but be added as receive data packet
// // (see the example of sending).
// DataPacket testPacket(51);


// // In this example communication will be via SoftwareSerial.
// // You can change this to any class that extend Stream class
// // (and put this class into streamComm instead of softSerial).
// SoftwareSerial softSerial(10, 11); // RX, TX

// // This is class from this library that uses any way of communication
// // that extend Stream class.
// // First paramter is pointer to the class extending Stream class.
// // The second one is maximum packet size in bytes + 1 (for packet ID)
// // If your communication way don't extend Stream class, you can implement
// // your own class that extend ITransceiver interface
// // and replace streamComm in comm instance.
// StreamComm streamComm(&softSerial, 20); // 20 is the max packet size in bytes (plus one byte for ID)

// // This is the main communication instance. It receives pointer to class that
// // extends ITransceiver interface. The second parameter is used to set the
// // maximum amount of queued data packets (needed if sending frequency is faster than receiving).
// // In NoQueuePacketCommunication there is no such parameter, so provide only the first one.
// PacketCommunicationWithQueue comm(&streamComm, 10);

// // Tasker is used to run tasks periodically.
// SimpleTasker tasker(5); // 5 is maximum amount of added tasks


// // You don't have to create separate functions to setup all data packets.
// // Just sent and received packets have to be created in the same way
// // (all byteType variables have to be added in the same order and ID have to be the same).
// void setupPacket()
// {
//     testPacket.addByteType(var1);
//     testPacket.addByteType(var2);
//     testPacket.addByteType(var3);
//     testPacket.addByteType(var4);
// }


// class SendDataTask : public Task
// {
//     void execute() override
//     {
//         // Fill data packet with some changing data
//         var1 = var1 + 1;
//         var2 = var2 + -2;
//         var3 = 123;
//         var4 = var4 + 10;

//         // Sending is just calling this method for a data packet we want to send.
//         // Sent are all variables previously added to the data packet using addByteType() method.
//         comm.sendDataPacket(&testPacket);
//     }
// };


// // Sending task instance
// SendDataTask sendDataTask;




