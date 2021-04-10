/**
 * @file PacketReceiver_example.ino
 * @author Jan Wielgus
 * @brief Example receiving data packets program.
 * Second device have to use ExamplePacketSender program.
 * This are only examples but both devices could send and receive
 * data in the same time.
 * @date 2020-11-03
 * 
 */


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
// Each received data packet have to have data packets constructed exactly
// in the same way and has the same ID as the received packet
// in the other device's code
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


class DataReceivedEvent : public IExecutable
{
    uint16_t counter = 0;

    void execute() override
    {
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
        Serial.print("\tStability: ");
        Serial.println(comm.getConnectionStability());
    }
};


// This event will be called right after data packet of this type
// will be received (if was added as packetReceivedEvent to the data packet).
DataReceivedEvent dataReceivedEvent;


void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    setupPacket();

    // Add all data packets that will be received usig this method.
    comm.addReceiveDataPacketPointer(&testPacket);


    // Add main communication class to the tasker and
    // set it's update frequency. It will periodically check
    // if there are any new data ready to receive,
    // and if data packet was received, it's packetReceivedEvent will be called.
    tasker.addTask(&comm, 1.f, 0);
    comm.adaptConnectionStabilityToInterval(); // use after adding to tasker if you are going to measure connection stability


    // Add packetReceivedEvent to all received data packets.
    // packetReceivedEvent is called always when specific data packet is received.
    testPacket.setPacketReceivedEvent(dataReceivedEvent);
}


void loop()
{
    tasker.runLoop();
}

