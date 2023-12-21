#ifndef BOND_EXECUTION_SERVICE_LISTENER_HPP
#define BOND_EXECUTION_SERVICE_LISTENER_HPP

#include "../executionservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"
#include "BondExecutionService.hpp"

class BondExecutionServiceListener : public ServiceListener<AlgoExecution<Bond>>
{
    private:
        BondExecutionService* service;

    public:
        BondExecutionServiceListener(BondExecutionService* _service);
        virtual void ProcessAdd(AlgoExecution<Bond>& data); // process an add event
        virtual void ProcessRemove(AlgoExecution<Bond>& data); // process a remove event
        virtual void ProcessUpdate(AlgoExecution<Bond>& data); // process an update event
};

// implementation

BondExecutionServiceListener::BondExecutionServiceListener(BondExecutionService* _service) : service(_service) {}

void BondExecutionServiceListener::ProcessAdd(AlgoExecution<Bond>& data) {service->AddAlgoExecution(data);}

void BondExecutionServiceListener::ProcessRemove(AlgoExecution<Bond>& data) {}

void BondExecutionServiceListener::ProcessUpdate(AlgoExecution<Bond>& data) {}

#endif