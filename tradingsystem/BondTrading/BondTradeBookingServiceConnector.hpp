#ifndef BOND_TRADE_BOOKING_SERVICE_CONNECTOR_HPP
#define BOND_TRADE_BOOKING_SERVICE_CONNECTOR_HPP

#include "BondTradeBookingService.hpp"

#include "../soa.hpp"
#include "../executionservice.hpp"
#include "../tradebookingservice.hpp"
#include "../utility.hpp"
#include "../bonds.hpp"

#include <fstream>

class BondTradeBookingServiceConnector : public Connector<Trade<Bond>>
{
    private:
	    BondTradeBookingService* service;   
    public:
        BondTradeBookingServiceConnector(BondTradeBookingService* _service);
        virtual void Publish(Connector<Bond>& trade);
        virtual void Subscribe(std::string filePath);
};


// implementation

BondTradeBookingServiceConnector::BondTradeBookingServiceConnector(BondTradeBookingService* _service) : service(_service) {}

void BondTradeBookingServiceConnector::Publish(Connector<Bond>& trade) {}

void BondTradeBookingServiceConnector::Subscribe(std::string filePath)
{
	std::ifstream file;
	std::string line, CUSIP, tradeId, priceStr, book, quantityStr, sideStr;
	double price;
	long quantity;
	Side side;

	//read file
	file.open(filePath);
	if (!file)
	{
		std::cerr << "Error: file cannot be opened.\n";
	}

	while (!file.eof())
	{
		file >> line; // sets EOF flag if no value found
		std::vector<std::string> info = tokenize(line, ',');
		CUSIP = info[0]; tradeId = info[1]; priceStr = info[2];
		book = info[3]; quantityStr = info[4]; sideStr = info[5];

		Bond product = CUSIP_to_BOND[CUSIP];
		price = StringToNum(priceStr);
		quantity = std::stoi(quantityStr);
		side = (sideStr == "BUY" ? BUY : SELL);

		Trade<Bond> trade(product, tradeId, price, book, quantity, side);
		service->OnMessage(trade);
	}

	file.close();
	std::cout << "Finished reading file " << filePath << "\n";
}















#endif