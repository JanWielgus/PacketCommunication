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
#include <FC_Communication_Base.h>
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

// Create communication handler
FC_CommunicationHandler comHandler(&mySerial, 30); // 30 is maximum amount of packet variables in bytes
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
        static bool bol;
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

    // Add communication handler to the tasker to update receiving
    // Receives all data to queues (each packet has its own one)
    // If queue is full and there are new data of that type, the oldest data of that type is removed
    // Updates values in the previously added receive data packet only with the oldest data in queues
    tasker.addTask(&comHandler, 500000L, 0); // 2Hz (receiving)

    // Add other tasks
    tasker.addTask(new sendData, 2000000L, 0);// 0.5Hz (sending)
    tasker.addTask(new showReceiveData, 500000L, 0);

    // Used only when using Tasker. Makes that time has not influence connection stability value.
    comHandler.adaptConStabFilterToInterval();


    // Add all pointers to data packets that will be received during communication
    // Variables inside that packet will be automatically updated
    // (when comHandler is added to the Tasker OR execute() method is called)
    comHandler.addRaceiveDataPacketPointer(&receiveDataPacket, 10);
    // .. other data packets that will be received


    // Assign startup values (testing)
    toSendDataPacket.var1 = 3;
    toSendDataPacket.var2 = 4;
    toSendDataPacket.var3 = 5;
    toSendDataPacket.var4 = 6;
    toSendDataPacket.var5 = 7;
}

void loop()
{
    tasker.run();
}
