#ifndef BOND_PRICING_SERVICE_CONNECTOR_HPP
#define BOND_PRICING_SERVICE_CONNECTOR_HPP

#include "soa.hpp"
#include "products.hpp"
#include "bondPricingService.hpp"
#include "pricingservice.hpp"

#include "utility.hpp"
#include "bonds.hpp"

#include <fstream>
#include <string>
#include <iostream>

using std::getline;

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
		std::cerr << "Error: file cannot be opened." << std::endl;
	}

	while (!file.eof())
	{
		file >> line; // sets EOF flag if no value found
		info = tokenize(line, ','); //"{CUSIP},{to_string(digitsMid)},{to_string(digitsSpread)}\n"
		CUSIP = info[0];
		priceMidStr = info[1];
		spreadStr = info[2];

		Bond product = CUSIP_to_BOND[CUSIP];
		priceMid = StringToNum(priceMidStr);
		spread = StringToNum(spreadStr);

		Price<Bond> price(product, priceMid, spread);

		service->OnMessage(price); // tell BondPricingService

	}
	file.close();
	std::cout << "Finished reading file " << filePath << endl;
}

#endif