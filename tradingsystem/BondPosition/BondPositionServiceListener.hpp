#ifndef BOND_POSITION_SERVICE_LISTENER_HPP
#define BOND_POSITION_SERVICE_LISTENER_HPP

#include "../positionservice.hpp"
#include "../products.hpp"
#include "../soa.hpp"
#include "../tradebookingservice.hpp"
#include "BondPositionService.hpp"

class BondPositionServiceListener : public ServiceListener<Trade<Bond>>
{
    private:
        BondPositionService* service;

    public:
        BondPositionServiceListener(BondPositionService* _service);
        virtual void ProcessAdd(Trade<Bond>& data); // process an add event
        virtual void ProcessRemove(Trade<Bond>& data); // process a remove event
        virtual void ProcessUpdate(Trade<Bond>& data); // process an update event
};

// implementation

BondPositionServiceListener::BondPositionServiceListener(BondPositionService* _service) : service(_service) {}
void BondPositionServiceListener::ProcessAdd(Trade<Bond>& data) {service->AddTrade(data);}
void BondPositionServiceListener::ProcessRemove(Trade<Bond>& data) {}
void BondPositionServiceListener::ProcessUpdate(Trade<Bond>& data) {}

#endif