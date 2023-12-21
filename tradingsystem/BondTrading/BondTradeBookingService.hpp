#ifndef BOND_TRADE_BOOKING_SERVICE_HPP
#define BOND_TRADE_BOOKING_SERVICE_HPP

#include "../tradebookingservice.hpp"
#include "../products.hpp"

class BondTradeBookingService : public TradeBookingService<Bond>
{
    private:
        std::map<std::string, Trade<Bond>> tradeMap;
        std::vector<ServiceListener<Trade<Bond>>*> listeners;

    public:
        BondTradeBookingService();
        virtual Trade<Bond>& GetData(std::string key); // get data given a key on service
        virtual void OnMessage(Trade<Bond>& data); // callback for any updated or new data
        virtual void AddListener(ServiceListener<Trade<Bond>>* listener); // add listener
        virtual const vector< ServiceListener<Trade<Bond>>*>& GetListeners() const; // get listeners
        void BookTrade(Trade<Bond>& trade); // book a trade
};

// implementation

BondTradeBookingService::BondTradeBookingService() : tradeMap(std::map<std::string, Trade<Bond>>()), listeners(std::vector<ServiceListener<Trade<Bond>>*>()) {}

Trade<Bond>& BondTradeBookingService::GetData(std::string key) {return tradeMap.at(key);}

void BondTradeBookingService::OnMessage(Trade<Bond>& data) {BookTrade(data);}

void BondTradeBookingService::AddListener(ServiceListener<Trade<Bond>>* listener) {listeners.push_back(listener);}

const vector<ServiceListener<Trade<Bond>>*>& BondTradeBookingService::GetListeners() const {return listeners;}

void BondTradeBookingService::BookTrade(Trade<Bond>& trade)
{
	std::string id = trade.GetTradeId();
	auto it = tradeMap.find(id);
	if (it != tradeMap.end())
	{
		tradeMap.erase(id);
	}
	tradeMap.insert(std::pair<std::string, Trade<Bond>>(id, trade));

	for (auto& listener : listeners)
	{
		listener->ProcessAdd(trade);
	}
}

#endif