/**
 * @file IConnectionStatus.h
 * @author Jan Wielgus
 * @date 2020-07-30
 * 
 */

#ifndef ICONNECTIONSTATUS_H
#define ICONNECTIONSTATUS_H


namespace Interfaces
{
    class IConnectionStatus
    {
    public:
        virtual ~IConnectionStatus() {}

        virtual uint8_t getConnectionStability() = 0;
        // TODO: think about other connection statuses (maybe boolean equivalent of method above)
    };
}


#endif
