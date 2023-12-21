#ifndef BOND_POSITION_SERVICE_HPP
#define BOND_POSITION_SERVICE_HPP

#include "../positionservice.hpp"
#include "../products.hpp"

class BondPositionService : public PositionService<Bond>
{
public:
	BondPositionService();

	// get data on our service provided a key
	virtual Position<Bond>& GetData(std::string key);

	// callback that a connector should invoke for any new or updated data
	virtual void OnMessage(Position<Bond>& data);

	// add a listener to the service for callbacks for data
	virtual void AddListener(ServiceListener<Position<Bond>>* listener);

	// get listeners on service
	virtual const vector< ServiceListener<Position<Bond>>*>& GetListeners() const;

	// add trade
	virtual void AddTrade(Trade<Bond>& trade);

private:
	std::map<std::string, Position<Bond>> positionMap;
	std::vector<ServiceListener<Position<Bond>>*> listeners;
};

// implementation 

BondPositionService::BondPositionService() : positionMap(std::map<std::string, Position<Bond>>()), listeners(std::vector<ServiceListener<Position<Bond>>*>()) {}

Position<Bond>& BondPositionService::GetData(std::string key) {return positionMap.at(key);}

void BondPositionService::OnMessage(Position<Bond>& data)
{
	// does not need a connector
}

void BondPositionService::AddListener(ServiceListener<Position<Bond>>* listener) {listeners.push_back(listener);}

const vector<ServiceListener<Position<Bond>>*>& BondPositionService::GetListeners() const {return listeners;}

void BondPositionService::AddTrade(Trade<Bond>& trade)
{
	string book = trade.GetBook();
	long quantity = trade.GetQuantity();
	long sign;
	if (trade.GetSide() == BUY)
	{
		sign = 1;
	}
	else
	{
		sign = -1;
	}
	quantity *= sign;

	string id = trade.GetProduct().GetProductId();
	auto it = positionMap.find(id);
	if (it != positionMap.end())
	{
		it->second.AddPosition(book, quantity);
	}
	else
	{
		// if id is not found, first add id and then AddTrade
		positionMap.insert(std::pair<std::string, Position<Bond>>(id, Position<Bond>(trade.GetProduct())));
		it = positionMap.find(id);
		it->second.AddPosition(book, quantity);
	}

	// call ProcessAdd on all listeners
	for (auto& listener : listeners)
	{
		listener->ProcessAdd(it->second);
	}
}


#endif