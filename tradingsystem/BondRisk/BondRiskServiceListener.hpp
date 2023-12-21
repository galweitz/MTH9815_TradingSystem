#ifndef BOND_RISK_SERVICE_LISTENER_HPP
#define BOND_RISK_SERVICE_LISTENER_HPP

#include "../riskservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"
#include "../BondPosition/BondPositionService.hpp"
#include "BondRiskService.hpp"

#include <map>
#include <vector>
#include <string>

class BondRiskServiceListener : public ServiceListener<Position<Bond>>
{
    private:
        BondRiskService* service;

    public:
        BondRiskServiceListener(BondRiskService* _service);
        virtual void ProcessAdd(Position<Bond>& data); // add an event
        virtual void ProcessRemove(Position<Bond>& data); // remove an event
        virtual void ProcessUpdate(Position<Bond>& data); // update an event
};

//implementation

BondRiskServiceListener::BondRiskServiceListener(BondRiskService* _service) : service(_service) {}

void BondRiskServiceListener::ProcessAdd(Position<Bond>& data) {service->AddPosition(data);}

void BondRiskServiceListener::ProcessRemove(Position<Bond>& data) {}

void BondRiskServiceListener::ProcessUpdate(Position<Bond>& data) {}

#endif