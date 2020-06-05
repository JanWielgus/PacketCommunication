/*
    Name:       FC_CommunicationHandler_example.ino
    Created:	10/04/2020
    Author:     Jan Wielgus
*/

#include <FC_ObjectTasker.h>
#include <FC_CustomDataTypes.h>
#include <FC_Task.h>
#include <FC_EVA_Filter.h>
#include <FC_SinkingQueue.h>
#include <FC_GrowingArray.h>
#include <ITransferable.h>
#include <DataBuffer.h>
#include <IPacketTransceiver.h>
#include <FC_SerialCommBase.h>
#include <DataPacketBase.h>
#include <FC_CommunicationHandler.h>
#include "ExampleDataPackets.h"

// To test on the software serial
#include <SoftwareSerial.h>


// Create instance of the software serial
SoftwareSerial mySerial(10, 11); // RX, TX

// Object tasker instance
FC_ObjectTasker tasker(5);

// Create instances of used data packets
RecDP_TestReceivePacket1 receiveDataPacket;
SenDP_TestToSendPacket1 toSendDataPacket;

// Create low-level communicaiton (which implement IPacketTransceiver interface)
FC_SerialCommBase serialCommBase(&mySerial, 30); // 30 is maximum packet size (size of uint8_t array)

// Create communication handler
FC_CommunicationHandler comHandler(&serialCommBase);
// For example uint32_t is 4 bytes, int16_t is 2 bytes
//(calculate the sum of all variables size in bytes and add a little for margin of error)


class showReceiveData : public FC_Task
{
    uint16_t number = 0; // showed data counter

    void execute() override
    {
        // Data can be always accessed.
        // Its values changes when comHandler is executed by tasker or executed manually

        Serial.print(number++);
        Serial.print(" Data: ");
        Serial.print("\t1: ");
        Serial.print(receiveDataPacket.var1);
        Serial.print("\t2: ");
        Serial.print(receiveDataPacket.var2);
        Serial.print("\t3: ");
        Serial.print(receiveDataPacket.var3);
        Serial.print("\t4: ");
        Serial.print(receiveDataPacket.var4);
        Serial.print("\t5: ");
        Serial.print(receiveDataPacket.var5);
        Serial.print("\tStability: ");
        Serial.println(comHandler.getConnectionStability());
    }
};

class sendData : public FC_Task
{
    void execute() override
    {
        // Update data that will be sent (just for testing changes it a little bit)
        toSendDataPacket.var1 += 2;
        toSendDataPacket.var2 -= 6;
        toSendDataPacket.var3 = toSendDataPacket.var1 + 11;
        toSendDataPacket.var4 = toSendDataPacket.var2 - 5;
        toSendDataPacket.var5 = toSendDataPacket.var4;

        // Blink a diode to indicate sending (if program got stuck this diode will not blink any more)
        static bool bol = true;
        digitalWrite(LED_BUILTIN, bol);
        bol = !bol;


        // Pass pointer to the class that inherits from ITransferable interface
        // It is immediately sent
        comHandler.sendDataPacket(&toSendDataPacket);
    }
};


void setup()
{
    // Debug serial port
    Serial.begin(115200);
    Serial.println("Program has just started");

    // Set builting diode as output (indicate sending)
    pinMode(LED_BUILTIN, OUTPUT);

    // Begin software serial (used by CommunicationHandler)
    mySerial.begin(9600);
	
	// Begin base communication
	serialCommBase.begin();

    // Add communication handler to the tasker to update receiving
    // Receives all data to queues (each packet has its own one)
    // If queue is full and there are new data of that type, the oldest data of that type is removed
    // Updates values in the previously added receive data packet only with the oldest data in queues
    tasker.addTask(&comHandler, 500000L, 0); // 2Hz (receiving)

    // Add other tasks
    tasker.addTask(new sendData, 2000000L, 0);// 0.5Hz (sending)
    // ...

    // Showing received data will be a packet event (automatically called when new data will be received)
    receiveDataPacket.setPacketEvent(new showReceiveData);

    // Used only when using Tasker. Makes that time has not influence connection stability value.
    comHandler.adaptConStabFilterToInterval();
    //comHandler.setConStabFilterIntensity(0.5); // other way to set up conneciton stability manually (if want smoothness other than from method above)


    // Add all pointers to data packets that will be received during communication
    // Variables inside that packet will be automatically updated
    // (when comHandler is added to the Tasker OR execute() method is called)
    comHandler.addRaceiveDataPacketPointer(&receiveDataPacket, 10);
    // .. other data packets that will be received


    // Assign initial values (testing)
    toSendDataPacket.var1 = 3;
    toSendDataPacket.var2 = 4;
    toSendDataPacket.var3 = 5;
    toSendDataPacket.var4 = 6;
    toSendDataPacket.var5 = 7.0f;
}

void loop()
{
    tasker.run();
}
