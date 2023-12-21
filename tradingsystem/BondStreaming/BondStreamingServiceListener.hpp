#ifndef BOND_STREAMING_SERVICE_LISTENER_HPP
#define BOND_STREAMING_SERVICE_LISTENER_HPP

#include "../soa.hpp"
#include "../products.hpp"
#include "../streamingservice.hpp"
#include "../BondAlgoStreaming/BondAlgoStreamingService.hpp"
#include "../BondStreaming/BondStreamingService.hpp"

class BondStreamingServiceListener : public ServiceListener<AlgoStream<Bond>>
{
    private:
        BondStreamingService* service;
        
    public:
        BondStreamingServiceListener(BondStreamingService* _service);
        virtual void ProcessAdd(AlgoStream<Bond>& data); // process add event
        virtual void ProcessRemove(AlgoStream<Bond>& data); // process remove event
        virtual void ProcessUpdate(AlgoStream<Bond>& data); // process update event
};

// implementation

BondStreamingServiceListener::BondStreamingServiceListener(BondStreamingService* _service) : service(_service) {}

void BondStreamingServiceListener::ProcessAdd(AlgoStream<Bond>& data) {service->AddAlgoStream(data);}

void BondStreamingServiceListener::ProcessRemove(AlgoStream<Bond>& data) {}

void BondStreamingServiceListener::ProcessUpdate(AlgoStream<Bond>& data) {}

#endif