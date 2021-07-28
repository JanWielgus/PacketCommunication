/**
 * @file PacketSender.ino
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Example sending program of data in data packets.
 * Second device have to use PacketReceiver program.
 * @date 2021-07-27
 */

#include <DataPacket.h>
#include <StreamComm.h>
#include <PacketCommunication.h>
#include <SoftwareSerial.h>
#include <SimpleTasker.h>
#include <byteType.h>

using namespace PacketComm;

const uint16_t MaxBufferSize = 20;
const float commSendingFrequency = 1.f;


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

    tasker.addTask(&sendDataTask, commSendingFrequency); // 1Hz sending tasker
}


void loop()
{
    tasker.loop();
}
