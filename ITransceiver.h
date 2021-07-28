/**
 * @file ITransceiver.h
 * @author Jan Wielgus
 * @brief Interface for all classes that can
 * send and receive uint8_t arrays (buffers).
 * Combine ITransmitter and IReceiver.
 * @date 2020-08-04
 */

#ifndef ITRANSCEIVER_H
#define ITRANSCEIVER_H

#include "ITransmitter.h"
#include "IReceiver.h"


namespace PacketComm
{
    /**
     * @brief Iterface for classes that enables
     * sending and receiving array of uint8_t values.
     * Classes that implements this interface are used in
     * packet based communication as low-level comm way.
     */
    class ITransceiver : public ITransmitter, public IReceiver
    {
    public:
        virtual ~ITransceiver() {}
    };
}


#endif
