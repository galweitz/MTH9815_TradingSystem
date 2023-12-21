#ifndef BOND_GENERIC_HISTORICAL_DATA_SERVICE_LISTENER_HPP
#define BOND_GENERIC_HISTORICAL_DATA_SERVICE_LISTENER_HPP

#include "../historicaldataservice.hpp"
#include "../positionservice.hpp"
#include "../riskservice.hpp"
#include "../executionservice.hpp"
#include "../streamingservice.hpp"
#include "../inquiryservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"

#include "BondHistoricalDataService.hpp"

#include <vector>
#include <map>
#include <string>


template <typename T>
class PositionHistoricalDataServiceListener : public ServiceListener<Position<T>>
{
    private:
        PositionHistoricalDataService<T>* service;

    public:
        PositionHistoricalDataServiceListener(PositionHistoricalDataService<T>* _service);

        virtual void ProcessAdd(Position<T>& data); // add an event
        virtual void ProcessRemove(Position<T>& data); // remove an evnt
        virtual void ProcessUpdate(Position<T>& data); // update an event

};

template <typename T>
class RiskHistoricalDataServiceListener : public ServiceListener<PV01<T>>
{
    private:
        RiskHistoricalDataService<T>* service;

    public:
        RiskHistoricalDataServiceListener(RiskHistoricalDataService<T>* _service);

        virtual void ProcessAdd(PV01<T>& data); // add an event
        virtual void ProcessRemove(PV01<T>& data); // remove an event
        virtual void ProcessUpdate(PV01<T>& data); // update an event
};

template <typename T>
class ExecutionHistoricalDataServiceListener : public ServiceListener<ExecutionOrder<T>>
{
    private:
        ExecutionHistoricalDataService<T>* service;

    public:
        ExecutionHistoricalDataServiceListener(ExecutionHistoricalDataService<T>* _service);
        virtual void ProcessAdd(ExecutionOrder<T>& data); // add an event
        virtual void ProcessRemove(ExecutionOrder<T>& data); // remove an event
        virtual void ProcessUpdate(ExecutionOrder<T>& data); // update an event
};

template <typename T>
class StreamingHistoricalDataServiceListener : public ServiceListener<PriceStream<T>>
{
    private:
        StreamingHistoricalDataService<T>* service;

    public:
        StreamingHistoricalDataServiceListener(StreamingHistoricalDataService<T>* _service);
        virtual void ProcessAdd(PriceStream<T>& data); // add an event
        virtual void ProcessRemove(PriceStream<T>& data); // remove an event
        virtual void ProcessUpdate(PriceStream<T>& data); // update an event
};

template <typename T>
class InquiryHistoricalDataServiceListener : public ServiceListener<Inquiry<T>>
{
    private:
        InquiryHistoricalDataService<T>* service;

    public:
        InquiryHistoricalDataServiceListener(InquiryHistoricalDataService<T>* _service);
        virtual void ProcessAdd(Inquiry<T>& data); // add an event
        virtual void ProcessRemove(Inquiry<T>& data); // remove an event
        virtual void ProcessUpdate(Inquiry<T>& data); // update an event
};

// implementation
//////////////////////////////////////////////////////////////////////////////////////

template <typename T>
PositionHistoricalDataServiceListener<T>::PositionHistoricalDataServiceListener(PositionHistoricalDataService<T>* _service): service(_service) {}

template <typename T>
void PositionHistoricalDataServiceListener<T>::ProcessAdd(Position<T>& data) {service->PersistData("", data);}

template <typename T>
void PositionHistoricalDataServiceListener<T>::ProcessRemove(Position<T>& data) {}

template <typename T>
void PositionHistoricalDataServiceListener<T>::ProcessUpdate(Position<T>& data) {}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
RiskHistoricalDataServiceListener<T>::RiskHistoricalDataServiceListener(RiskHistoricalDataService<T>* _service) : service(_service) {}

template <typename T>
void RiskHistoricalDataServiceListener<T>::ProcessAdd(PV01<T>& data) {service->PersistData("", data);}

template <typename T>
void RiskHistoricalDataServiceListener<T>::ProcessRemove(PV01<T>& data) {}

template <typename T>
void RiskHistoricalDataServiceListener<T>::ProcessUpdate(PV01<T>& data) {}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
ExecutionHistoricalDataServiceListener<T>::ExecutionHistoricalDataServiceListener(ExecutionHistoricalDataService<T>* _service) : service(_service) {}

template <typename T>
void ExecutionHistoricalDataServiceListener<T>::ProcessAdd(ExecutionOrder<T>& data) {service->PersistData("", data);}

template <typename T>
void ExecutionHistoricalDataServiceListener<T>::ProcessRemove(ExecutionOrder<T>& data) {}

template <typename T>
void ExecutionHistoricalDataServiceListener<T>::ProcessUpdate(ExecutionOrder<T>& data) {}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
StreamingHistoricalDataServiceListener<T>::StreamingHistoricalDataServiceListener(StreamingHistoricalDataService<T>* _service) : service(_service) {}


template <typename T>
void StreamingHistoricalDataServiceListener<T>::ProcessAdd(PriceStream<T>& data) {service->PersistData("", data);}

template <typename T>
void StreamingHistoricalDataServiceListener<T>::ProcessRemove(PriceStream<T>& data) {}

template <typename T>
void StreamingHistoricalDataServiceListener<T>::ProcessUpdate(PriceStream<T>& data) {}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
InquiryHistoricalDataServiceListener<T>::InquiryHistoricalDataServiceListener(InquiryHistoricalDataService<T>* _service) : service(_service) {}

template <typename T>
void InquiryHistoricalDataServiceListener<T>::ProcessAdd(Inquiry<T>& data) {service->PersistData("", data);}

template <typename T>
void InquiryHistoricalDataServiceListener<T>::ProcessRemove(Inquiry<T>& data) {}

template <typename T>
void InquiryHistoricalDataServiceListener<T>::ProcessUpdate(Inquiry<T>& data) {}

#endif