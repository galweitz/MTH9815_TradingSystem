#ifndef BOND_STREAMING_SERVICE_HPP
#define BOND_STREAMING_SERVICE_HPP

#include "../streamingservice.hpp"
#include "../BondAlgoStreaming/BondAlgoStreamingService.hpp"
#include "../soa.hpp"
#include "../products.hpp"
#include "BondStreamingServiceConnector.hpp"
#include <map>
#include <vector>
#include <string>


class BondStreamingService : public StreamingService<Bond>
{
    private:
        std::map<std::string, PriceStream<Bond>> pricestreamMap;
        std::vector<ServiceListener<PriceStream<Bond>>*> listeners;
        BondStreamingServiceConnector* connector;
        
    public:
        BondStreamingService(); // param ctor
        virtual PriceStream<Bond>& GetData(std::string key); // get data given a key
        BondStreamingServiceConnector& GetConnector() const; // get the connector
        virtual void OnMessage(PriceStream<Bond>& data); // the callback that a Connector should invoke for any new or updated data
        virtual void AddListener(ServiceListener<PriceStream<Bond>>* listener); // add a listener
        virtual const vector<ServiceListener<PriceStream<Bond>>*>& GetListeners() const; // get all listeners
        virtual void AddAlgoStream(const AlgoStream<Bond>& price); // when listener gets a new Price
        virtual void PublishPrice(PriceStream<Bond>& priceStream); // call connector to publish
};

// implementation

BondStreamingService::BondStreamingService():
	pricestreamMap(std::map<std::string, PriceStream<Bond>>()), listeners(std::vector<ServiceListener<PriceStream<Bond>>*>()), connector(new BondStreamingServiceConnector()) {}

// Get data on our service given a key
PriceStream<Bond>& BondStreamingService::GetData(std::string key) {return pricestreamMap.at(key);}

// get the connector
BondStreamingServiceConnector& BondStreamingService::GetConnector() const
{
	return *connector;
}

// The callback that a Connector should invoke for any new or updated data
void BondStreamingService::OnMessage(PriceStream<Bond>& data)
{

}

// Add a listener to the Service for callbacks on add, remove, and update events
// for data to the Service.
void BondStreamingService::AddListener(ServiceListener<PriceStream<Bond>>* listener)
{
	listeners.push_back(listener);
}

// Get all listeners on the Service.
const vector<ServiceListener<PriceStream<Bond>>*>& BondStreamingService::GetListeners() const
{
	return listeners;
}

// when listener gets a new Price
void BondStreamingService::AddAlgoStream(const AlgoStream<Bond>& price)
{
	PriceStream<Bond> priceStream = price.GetPriceStream();
	Bond product = priceStream.GetProduct();
	std::string id = product.GetProductId();
	auto it = pricestreamMap.find(id);
	if (it != pricestreamMap.end())
	{
		pricestreamMap.erase(id);
	}
	pricestreamMap.insert(std::pair<std::string, PriceStream<Bond>>(id, priceStream));

	PublishPrice(priceStream); // tell connector to publish the priceStream

	for (auto& listener : listeners)
	{
		listener->ProcessAdd(priceStream); // tell BondHistoricalDataServiceListener
	}
}



void BondStreamingService::PublishPrice(PriceStream<Bond>& priceStream)
{
	connector->Publish(priceStream);
}







#endif