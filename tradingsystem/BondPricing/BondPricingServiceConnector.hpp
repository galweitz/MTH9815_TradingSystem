#ifndef BOND_PRICING_SERVICE_CONNECTOR_HPP
#define BOND_PRICING_SERVICE_CONNECTOR_HPP

#include "BondPricingService.hpp"

#include "../utility.hpp"
#include "../bonds.hpp"

#include <fstream>

class BondPricingServiceConnector : public Connector<Price<Bond>>
{
    private:
        BondPricingService* service;

    public:
        BondPricingServiceConnector(BondPricingService* _service);
        virtual void Publish(Price<Bond>& price);
        virtual void Subscribe(std::string filePath);
};

// implementation 

BondPricingServiceConnector::BondPricingServiceConnector(BondPricingService* _service): service(_service) {}

void BondPricingServiceConnector::Publish(Price<Bond>& price) {}

void BondPricingServiceConnector::Subscribe(std::string filePath)
{
	std::ifstream file;
	std::string line, CUSIP, priceMidStr, spreadStr;
	double priceMid, spread;
	std::vector<std::string> info;

	//read file
	file.open(filePath);
	if (!file)
	{
		std::cerr << "Error: file cannot be opened.\n";
	}

	while (!file.eof())
	{
		file >> line;
		info = tokenize(line, ',');
		CUSIP = info[0]; priceMidStr = info[1]; spreadStr = info[2];

		Bond product = CUSIP_to_BOND[CUSIP];
		priceMid = StringToNum(priceMidStr);
		spread = StringToNum(spreadStr);

		Price<Bond> price(product, priceMid, spread);

		service->OnMessage(price);

	}

	file.close();
	std::cout << "Finished reading file " << filePath << "\n";
}

#endif