/**
 * @file IConnectionStatus.h
 * @author Jan Wielgus
 * @date 2020-07-30
 */

#ifndef ICONNECTIONSTATUS_H
#define ICONNECTIONSTATUS_H

#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace PacketComm
{
    /**
     * @brief Interface for classes that enables to check
     * connection status.
     */
    class IConnectionStatus
    {
    public:
        virtual ~IConnectionStatus() {}

        /**
         * @brief Checks connection stability.
         * @return conneciton stability in % (0 - no conneciton, 100 - uninterrupted connection)
         */
        virtual uint8_t getConnectionStability() = 0;
        // TODO: think about other connection statuses (maybe boolean equivalent of method above)
    };
}


#endif
