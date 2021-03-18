/**
 * @file ReceivedDataObserverDecorator.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief This class is used to be able to register additional
 * tasks that will be notified when new data will be received by
 * primary receiver.
 * @date 2021-03-18
 * 
 * 
 */

#ifndef RECEIVEDDATAOBSERVERSDECORATOR_H
#define RECEIVEDDATAOBSERVERSDECORATOR_H


#include "ITransceiver.h"
#include <GrowingArray.h>


class ReceivedDataObserversDecorator : public ITransceiver
{
public:
    class DataObserver
    {
    public:
        virtual ~DataObserver() {}

        virtual void newDataReceived(const DataBuffer& newData) = 0;
    };


private:
    ITransceiver& baseTransceiver;
    GrowingArray<DataObserver*> dataObservers;


public:
    ReceivedDataObserversDecorator(ITransceiver& _baseTransceiver)
        : baseTransceiver(_baseTransceiver)
    {
    }

    void begin()
    {
        baseTransceiver.begin();
    }

    bool send(const uint8_t* buffer, size_t size)
    {
        return baseTransceiver.send(buffer, size);
    }

    bool send(const DataBuffer& buffer)
    {
        return baseTransceiver.send(buffer);
    }

    bool send(const ExtendedDataBuffer& buffer)
    {
        return baseTransceiver.send(buffer);
    }

    bool receiveData()
    {
        bool successul_flag = baseTransceiver.receiveData();

        if (successul_flag)
            callDataObservers(baseTransceiver.getReceivedData());

        return successul_flag;
    }

    DataBuffer getReceivedData()
    {
        return baseTransceiver.getReceivedData();
    }

private:
    void callDataObservers(const DataBuffer& newData)
    {
        auto iter = dataObservers.getIterator();
        while (iter->hasNext())
            iter->next()->newDataReceived(newData);
    }
};


#endif
