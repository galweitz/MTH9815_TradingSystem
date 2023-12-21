#ifndef BOND_ALGO_EXECUTION_SERVICE_LISTENER_HPP
#define BOND_ALGO_EXECUTION_SERVICE_LISTENER_HPP

#include "../executionservice.hpp"
#include "../marketdataservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"
#include "BondAlgoExecutionService.hpp"

class BondAlgoExecutionServiceListener : public ServiceListener<OrderBook<Bond>>
{
    private:
	    BondAlgoExecutionService* service;

    public:
        BondAlgoExecutionServiceListener(BondAlgoExecutionService* _service);
        virtual void ProcessAdd(OrderBook<Bond>& data); // process an add event
        virtual void ProcessRemove(OrderBook<Bond>& data); // process a remove event
        virtual void ProcessUpdate(OrderBook<Bond>& data); // process an update event
};

// implementation 

BondAlgoExecutionServiceListener::BondAlgoExecutionServiceListener(BondAlgoExecutionService* _service): service(_service) {}

void BondAlgoExecutionServiceListener::ProcessAdd(OrderBook<Bond>& data) {service->AddOrderBook(data);}

void BondAlgoExecutionServiceListener::ProcessRemove(OrderBook<Bond>& data) {}

void BondAlgoExecutionServiceListener::ProcessUpdate(OrderBook<Bond>& data) {}

#endif