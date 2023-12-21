#ifndef BOND_ALGO_STREAMING_SERVICE_LISTENER_HPP
#define BOND_ALGO_STREAMING_SERVICE_LISTENER_HPP

#include "../soa.hpp"
#include "../streamingservice.hpp"
#include "../pricingservice.hpp"
#include "../products.hpp"
#include "BondAlgoStreamingService.hpp"

class BondAlgoStreamingServiceListener : public ServiceListener<Price<Bond>>
{
    private:
        BondAlgoStreamingService* service;
    
    public:
        BondAlgoStreamingServiceListener(BondAlgoStreamingService* _service);
        virtual void ProcessAdd(Price<Bond>& data); // process an add event
        virtual void ProcessRemove(Price<Bond>& data); // process a remove event
        virtual void ProcessUpdate(Price<Bond>& data); // process an update event
};

//implementation

BondAlgoStreamingServiceListener::BondAlgoStreamingServiceListener(BondAlgoStreamingService* _service) : service(_service) {}
void BondAlgoStreamingServiceListener::ProcessAdd(Price<Bond>& data) {service->AddPrice(data);}
void BondAlgoStreamingServiceListener::ProcessRemove(Price<Bond>& data) {}
void BondAlgoStreamingServiceListener::ProcessUpdate(Price<Bond>& data) {}

#endif