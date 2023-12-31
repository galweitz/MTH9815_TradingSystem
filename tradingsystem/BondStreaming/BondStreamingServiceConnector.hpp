#ifndef BOND_STREAMING_SERVICE_CONNECTOR_HPP
#define BOND_STREAMING_SERVICE_CONNECTOR_HPP

#include "../soa.hpp"
#include "../products.hpp"
#include "../streamingservice.hpp"
#include "../BondAlgoStreaming/BondAlgoStreamingService.hpp"

class BondStreamingServiceConnector : public Connector<PriceStream<Bond>>
{
    private:
        static int i;

    public:
        virtual void Publish(PriceStream<Bond>& orderBook);
        virtual void Subscribe(std::string filePath);
};

// implementation

int BondStreamingServiceConnector::i = 0;

void BondStreamingServiceConnector::Publish(PriceStream<Bond>& priceStream)
{
	if (!(++i % 1000))
	{
		std::cout << i << "th Price Stream:" << std::endl;
		std::cout << priceStream.str() << std::endl;
	}
}

void BondStreamingServiceConnector::Subscribe(std::string filePath) {}

#endif