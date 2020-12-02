/**
 * @file ESP8266WiFiComm.h
 * @author Jan Wielgus
 * @brief WiFi communication using ESP8266.
 * @date 2020-12-02
 * 
 */

#ifndef ESP8266WIFICOMM_H
#define ESP8266WIFICOMM_H

#include "ITransceiver.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


class ESP8266WiFiComm : public ITransceiver
{
private:
    WiFiUDP udp;
    IPAddress localIPAddress;
    IPAddress targetIPAddress;

    bool sendAlwaysToLastSender_flag = false;
    bool updBeginned_flag = false;

    // initial config
	const char* const SSID;
	const char* const Password;
	const uint16_t Port;

    ExtendedDataBuffer receiveBuffer;


public:
    ESP8266WiFiComm(const char* ssid, const char* password, uint16_t port, size_t maxPacketSize);

    // interface
    void begin() override;
    bool send(const uint8_t* buffer, size_t size) override;
    bool send(const DataBuffer& buffer) override;
    bool send(const ExtendedDataBuffer& buffer) override;
    size_t available() override;
    DataBuffer receiveNextData() override;

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
    bool checkIfBeginnedUDP() const;
};


#endif

