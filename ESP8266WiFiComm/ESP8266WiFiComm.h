/**
 * @file ESP8266WiFiComm.h
 * @author Jan Wielgus
 * @brief WiFi communication using ESP8266.
 * @date 2020-12-02
 */

#ifndef ESP8266WIFICOMM_H
#define ESP8266WIFICOMM_H

#ifdef ESP8266

#include "ITransceiver.h"
#include "DataBuffer.h"
#include "AutoDataBuffer.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


namespace PacketComm
{
    class ESP8266WiFiComm : public ITransceiver
    {
    private:
        WiFiUDP udp;
        IPAddress localIPAddress;
        IPAddress targetIPAddress;

        bool sendAlwaysToLastSender_flag = true; // by default
        bool updBeginned_flag = false;

        // config
        const uint16_t Port;

        AutoDataBuffer receiveBuffer;


    public:
        ESP8266WiFiComm(uint16_t port, size_t maxPacketSize);

        bool send(const uint8_t* buffer, size_t size) override;
        bool receive() override;
        const DataBuffer getReceived() override;

        /**
         * @brief Set the IP address that all next packets will be send to.
         * @param ipAddress IP address.
         */
        void setTargetIPAddress(IPAddress ipAddress);

        /**
         * @brief Set the IP address that all next packets will be send to.
         * @param ipAddress IP address in octets divided form.
         */
        void setTargetIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);

        /**
         * @brief Packets always will be sent to the latest sender
         * (you have to receive at least one packet to send packets correctly).
         */
        void setTargetIPAddressAlwaysToSender();

        /**
         * @brief Check if WiFi connection was established. 
         */
        bool isConnected() const;

        /**
         * @return IP address of this device.
         */
        IPAddress getLocalIP() const;


    private:
        /**
         * @brief Should be called if wifi connection was established.
         * Starts listening on the local port.
         * @return false if WiFi connection was not established
         * or UDP was already successfully beginned,
         * otherwise returns true.
         */
        bool beginUDP();

        /**
         * @brief Return true if after connection being established
         * udp was beginned and started listening on the Port.
         */
        bool beginnedUDP() const;
    };



    inline bool ESP8266WiFiComm::beginnedUDP() const
    {
        return updBeginned_flag;
    }
}


#endif // ESP8266

#endif // ESP8266WIFICOMM_H
