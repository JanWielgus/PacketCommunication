/**
 * @file SerialLowLevelComm_example.ino
 * @author Jan Wielgus
 * @brief This is example that show how to use only lower level part of this library.
 * Upload the same code on two devices and connect them using software serial
 * on pins 10, 11 (RX, TX), or change communication library that extends ITransceiver interface.
 * You can send and receive array of uint8_ts (bytes).
 * ITransceiver interface is the base for all low-level communication ways.
 * You can write your own and it will work with data packet based communication.
 * @date 2020-10-29
 * 
 */

#include <StreamComm.h>
#include <SoftwareSerial.h>

void send();
void receive();


// Config
const uint16_t MaxPacketSize = 25;

// Used variables and other
uint8_t toSendBuffer[2];
uint16_t missedPackets = 0;



// Communicaiton between devices will be over software serial.
SoftwareSerial softSerial(10, 11); // RX, TX


// There is example for arduino stream communications using SoftwareSerial.
// StreamComm class can be used for any way of communication that extends Stream class.
// For example Serial.
StreamComm streamComm(&softSerial, MaxPacketSize);
ITransceiver& comm = streamComm;



void setup()
{
    Serial.begin(9600); // used for showing results
    softSerial.begin(9600); // used for communication
    comm.begin();
}


void loop()
{
    send();
    receive();
    delay(1000);
}



void send()
{
    // Some value that will change and indicate that communication is working.
    // You can send an array of size at most MaxPacketSize and put there
    // whatever you want to send (bigger types can be divided using ByteTypes library
    // https://github.com/JanWielgus/ByteTypes based on idea of Filip Depta)
    toSendBuffer[0] = (uint8_t)millis();
    toSendBuffer[1] = toSendBuffer[0] / 10;

    comm.send(toSendBuffer, 2); // in this example maximum send size is 25 (because of MaxPacketSize)
                                // but we will use only 2
}



void receive()
{
    bool receivedSomeDataFlag = false; // if this flag is false at the end, any data was received
    uint8_t var1;
    uint8_t var2;

    // Check if there are any data ready to receive.
    // But be aware that this data may be corrupted
    // and data packet cannot be created, so receiveNextData()
    // method will return empty buffer.
    // If you expect more data, you can check it in a loop.
    if (comm.available())
    {
        DataBuffer receivedData = comm.receiveNextData();

        // Check if received data buffer size is not zero
        if (receivedData.size > 0)
        {
            var1 = receivedData.buffer[0];
            var2 = receivedData.buffer[1];
            // size indicate amount of received bytes.
            // In this example we send only 2 bit buffers so I don't check.

            receivedSomeDataFlag = true;
        }
    }

    if (!receivedSomeDataFlag)
        missedPackets++;

    // Show receiving results
    Serial.println("Receiving done");
    Serial.print("Missed packets: ");
    Serial.println(missedPackets);
    if (receivedSomeDataFlag)
    {
        Serial.print("Received some data: ");
        Serial.print(var1);
        Serial.print('\t');
        Serial.println(var2);
    }
    else
        Serial.println("No data was received");
}

