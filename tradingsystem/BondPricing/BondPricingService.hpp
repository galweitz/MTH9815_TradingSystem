#ifndef BOND_PRICING_SERVICE_HPP
#define BOND_PRICING_SERVICE_HPP

#include <map>
#include <vector>

#include "../soa.hpp"
#include "../pricingservice.hpp"
#include "../products.hpp"


class BondPricingService : public PricingService<Bond>
{
	private:
		std::map<std::string, Price<Bond>> priceMap;
		std::vector<ServiceListener<Price<Bond>>*> listeners;

	public:
		BondPricingService();
		virtual Price<Bond>& GetData(std::string key); // get data on our service given a key		
		virtual void OnMessage(Price<Bond>& data); // the callback that a connector should invoke for any new or updated data
		virtual void AddListener(ServiceListener<Price<Bond>>* listener); // add listener
		virtual const vector< ServiceListener<Price<Bond>>*>& GetListeners() const; // get all listeners
};

// implementation

BondPricingService::BondPricingService() : priceMap(std::map<std::string, Price<Bond>>()), listeners(std::vector<ServiceListener<Price<Bond>>*>()) {}

Price<Bond>& BondPricingService::GetData(std::string key) {return priceMap.at(key);}

void BondPricingService::OnMessage(Price<Bond>& data)
{
	std::string id = data.GetProduct().GetProductId();
	auto it = priceMap.find(id);
	if (it != priceMap.end())
	{
		priceMap.erase(id);
	}
	priceMap.insert(std::pair<std::string, Price<Bond>>(id, data));

	for (auto& listener : listeners)
	{
		listener->ProcessAdd(data);
	}
}

void BondPricingService::AddListener(ServiceListener<Price<Bond>>* listener) {listeners.push_back(listener);}

const vector<ServiceListener<Price<Bond>>*>& BondPricingService::GetListeners() const {return listeners;}

#endif