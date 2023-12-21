#ifndef BOND_GUI_SERVICE_LISTENER_HPP
#define BOND_GUI_SERVICE_LISTENER_HPP

#include "../pricingservice.hpp"
#include "../products.hpp"
#include "../soa.hpp"

#include "BondGUIService.hpp"

class BondGUIServiceListener : public ServiceListener<Price<Bond>>
{
    private:
        BondGUIService* service;

    public:
        BondGUIServiceListener(BondGUIService* _service);
        virtual void ProcessAdd(Price<Bond>& data); // process an add event
        virtual void ProcessRemove(Price<Bond>& data); // process a remove event
        virtual void ProcessUpdate(Price<Bond>& data); // process an update event
};

// implementation 

BondGUIServiceListener::BondGUIServiceListener(BondGUIService* _service) : service(_service) {}
void BondGUIServiceListener::ProcessAdd(Price<Bond>& data) {service->OnMessage(data);}
void BondGUIServiceListener::ProcessRemove(Price<Bond>& data) {}
void BondGUIServiceListener::ProcessUpdate(Price<Bond>& data) {}

#endif