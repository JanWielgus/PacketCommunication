/**
 * @file PacketReceiver.ino
 * @author Jan Wielgus
 * @brief Example receiving data packets program.
 * Second device have to use ExamplePacketSender program.
 * This are only examples but both devices could send and receive
 * data in the same time.
 * @date 2020-11-03
 */

#include <DataPacket.h>
#include <StreamComm.h>
#include <PacketCommunication.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <byteType.h>

using namespace PacketComm;

const uint16_t MaxBufferSize = 20;
const float commReceivingFrequency = 1.f;

void dataReceivedCallback();


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
 * Packet in sender code have to be constructed in the same way
 * (important parts are: order in which variables are added to this packet
 * using addVar() method and packet ID).
 * You can create many data packets. Remember that they have to have different ID!
 * This packet ID is 51.
 * 
 * After ID there is optional parameter where you can set void function
 * that will be called each time this packet will be received
 * (this is useless if packet is only sent).
 * At the end one instance of this class (packet) is created.
 */
class TestPacket : public DataPacket
{
public:
    uint32Byte var1;
    uint8Byte var2;
    int16Byte var3;
    floatByte var4;

    uint16_t counter = 0; // used in received event

    TestPacket()
        : DataPacket(51, dataReceivedCallback)
    {
        addVar(var1);
        addVar(var2);
        addVar(var3);
        addVar(var4);
    }
} testPacket;


/**
 * Wrapping task to execute comm receive() method.
 */
class : public Task
{
    void execute() override
    {
        comm.receive();
    }
} commReceiveTask;



void setup()
{
    Serial.begin(9600);
    softSerial.begin(9600);

    Serial.println("Program has just started!");

    comm.registerReceivePacket(&testPacket);

    tasker.addTask(&commReceiveTask, commReceivingFrequency);
    comm.adaptConnStabilityToFrequency(commReceivingFrequency);
}


void loop()
{
    tasker.loop();
}



void dataReceivedCallback() 
{
    Serial.print(testPacket.counter++);
    Serial.print(" Data: ");
    Serial.print("\t1: ");
    Serial.print(testPacket.var1);
    Serial.print("\t2: ");
    Serial.print(testPacket.var2);
    Serial.print("\t3: ");
    Serial.print(testPacket.var3);
    Serial.print("\t4: ");
    Serial.print(testPacket.var4);
    Serial.print("\tStability: ");
    Serial.println(comm.getConnectionStability());
}
