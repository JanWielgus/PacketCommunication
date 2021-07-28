/**
 * @file SerialLowLevelComm.ino
 * @author Jan Wielgus
 * @brief This is example that show how to use only lower level part of this library.
 * Upload the same code on two devices and connect them using software serial
 * on pins 10, 11 (RX, TX), or change communication library that extends ITransceiver interface.
 * You can send and receive array of uint8_ts (bytes).
 * ITransceiver interface is the base for all low-level communication ways.
 * You can write your own and it will work with data packet based communication.
 * @date 2020-10-29
 */

#include <StreamComm.h>
#include <ITransceiver.h>
#include <SoftwareSerial.h>


void send();
void receive();

// Config
const uint16_t MaxBufferSize = 25;

// Used variables and other
uint8_t toSendBuffer[2];
uint16_t missedPackets = 0;

// Communicaiton between devices will be over software serial.
SoftwareSerial softSerial(10, 11); // RX, TX

/**
 * There is example for arduino stream communications using SoftwareSerial.
 * StreamComm class can be used for any way of communication that
 * extends Stream class (for example HardwareSeial).
 */
PacketComm::StreamComm<MaxBufferSize> streamComm(&softSerial);
PacketComm::ITransceiver& comm = streamComm; // you can use general interface



void setup()
{
    Serial.begin(9600); // used for showing results
    softSerial.begin(9600); // used for communication
}


void loop()
{
    send();
    receive();
    delay(1000);
}



void send()
{
    /**
     * Some value that will change and indicate that communication is working.
     * You can send an array of size at most MaxBufferSize and put there
     * whatever you want to send (bigger types than bytes can be divided using
     * ByteTypes library: https://github.com/JanWielgus/ByteTypes based on Filip Depta's idea)
     */
    toSendBuffer[0] = (uint8_t)millis();
    toSendBuffer[1] = toSendBuffer[0] / 10;

    comm.send(toSendBuffer, 2); // remember of the MaxBufferSize
}


void receive()
{
    uint8_t var1;
    uint8_t var2;

    // Check if any data has been received.
    if (comm.receive())
    {
        PacketComm::DataBuffer receivedData = comm.getReceived();

        /**
         * receivedData.size contains size of the received data,
         * but we know that in this example will be always 2.
         * Of course you can check it.
         */

        // You can store data somewhere.
        var1 = receivedData.buffer[0];
        var2 = receivedData.buffer[1];


        Serial.print("Received some data: ");
        Serial.print(var1);
        Serial.print('\t');
        Serial.println(var2);
    }
    else
    {
        missedPackets++;
        Serial.println("No data was received");
    }

    Serial.println("Receiving done");
    Serial.print("Total missed packets: ");
    Serial.println(missedPackets);
}

