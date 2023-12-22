#ifndef BOND_HISTORICAL_DATA_SERVICE_HPP
#define BOND_HISTORICAL_DATA_SERVICE_HPP

#include "../historicaldataservice.hpp"
#include "../positionservice.hpp"
#include "../riskservice.hpp"
#include "../executionservice.hpp"
#include "../streamingservice.hpp"
#include "../inquiryservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"

#include "BondHistoricalDataServiceConnector.hpp"

#include <vector>
#include <map>
#include <string>


template <typename T>
class PositionHistoricalDataService : public HistoricalDataService<Position<T>>
{
	private:
		std::map<std::string, Position<T>> dataMap;
		std::vector<ServiceListener<Position<T>>*> listeners;
		PositionHistoricalDataServiceConnector<T>* connector;

	public:
		PositionHistoricalDataService();
		virtual Position<T>& GetData(std::string key); // get data given a key
		virtual PositionHistoricalDataServiceConnector<T>& GetConnector() const; // get connector
		virtual void OnMessage(Position<T>& data); // callback that a Connector should invoke for any new or updated data
		virtual void AddListener(ServiceListener<Position<T>>* listener); // add listener
		virtual const vector< ServiceListener<Position<T>>*>& GetListeners() const; // get all listeners
		void PersistData(std::string persistKey, const Position<T>& data) override; // persist data to a store
};

template <typename T>
class RiskHistoricalDataService : public HistoricalDataService<PV01<T>>
{
	private:
		std::map<std::string, PV01<T>> dataMap;
		std::vector<ServiceListener<PV01<T>>*> listeners;
		RiskHistoricalDataServiceConnector<T>* connector;
		std::vector<BucketedSector<T>> sectors; // record bucketed sectors risks

	public:
		RiskHistoricalDataService();
		virtual PV01<T>& GetData(std::string key); // get data given a key
		virtual RiskHistoricalDataServiceConnector<T>& GetConnector() const; // get connector
		virtual void OnMessage(PV01<T>& data); // callback that a Connector should invoke for any new or updated data
		virtual void AddListener(ServiceListener<PV01<T>>* listener); // add listener
		virtual const vector< ServiceListener<PV01<T>>*>& GetListeners() const; // get all listeners
		virtual const PV01<BucketedSector<Bond>> GetBucketedRisk(const BucketedSector<Bond>& sector) const; // get bucketed risk for the bucket sector
		virtual void AddBucketedSector(const BucketedSector<T>& sector); // record bucketed sectors risks
		void PersistData(std::string persistKey, const PV01<T>& data) override; // persist data to a store
};

template <typename T>
class ExecutionHistoricalDataService : public HistoricalDataService<ExecutionOrder<T>>
{
	private:
		std::map<std::string, ExecutionOrder<T>> dataMap;
		std::vector<ServiceListener<ExecutionOrder<T>>*> listeners;
		ExecutionHistoricalDataServiceConnector<T>* connector;

	public:
		ExecutionHistoricalDataService();
		virtual ExecutionOrder<T>& GetData(std::string key);
		virtual ExecutionHistoricalDataServiceConnector<T>& GetConnector() const;
		virtual void OnMessage(ExecutionOrder<T>& data);
		virtual void AddListener(ServiceListener<ExecutionOrder<T>>* listener);
		virtual const vector< ServiceListener<ExecutionOrder<T>>*>& GetListeners() const;
		void PersistData(std::string persistKey, const ExecutionOrder<T>& data) override;
};


template <typename T>
class StreamingHistoricalDataService : public HistoricalDataService<PriceStream<T>>
{
	private:
		std::map<std::string, PriceStream<T>> dataMap;
		std::vector<ServiceListener<PriceStream<T>>*> listeners;
		StreamingHistoricalDataServiceConnector<T>* connector;

	public:
		StreamingHistoricalDataService();
		virtual PriceStream<T>& GetData(std::string key);
		virtual StreamingHistoricalDataServiceConnector<T>& GetConnector() const;
		virtual void OnMessage(PriceStream<T>& data);
		virtual void AddListener(ServiceListener<PriceStream<T>>* listener);
		virtual const vector< ServiceListener<PriceStream<T>>*>& GetListeners() const;
		void PersistData(std::string persistKey, const PriceStream<T>& data) override;
};


template <typename T>
class InquiryHistoricalDataService : public HistoricalDataService<Inquiry<T>>
{
	private:
		std::map<std::string, Inquiry<T>> dataMap;
		std::vector<ServiceListener<Inquiry<T>>*> listeners;
		InquiryHistoricalDataServiceConnector<T>* connector;

	public:
		InquiryHistoricalDataService();
		virtual Inquiry<T>& GetData(std::string key);
		virtual InquiryHistoricalDataServiceConnector<T>& GetConnector() const;
		virtual void OnMessage(Inquiry<T>& data);
		virtual void AddListener(ServiceListener<Inquiry<T>>* listener);
		virtual const vector< ServiceListener<Inquiry<T>>*>& GetListeners() const;
		void PersistData(std::string persistKey, const Inquiry<T>& data) override;
};

// implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
PositionHistoricalDataService<T>::PositionHistoricalDataService() :
	dataMap(std::map<std::string, Position<T>>()), listeners(std::vector<ServiceListener<Position<T>>*>()), connector(new PositionHistoricalDataServiceConnector<T>()) {}

template <typename T>
Position<T>& PositionHistoricalDataService<T>::GetData(std::string key) {return dataMap.at(key);}

template <typename T>
PositionHistoricalDataServiceConnector<T>& PositionHistoricalDataService<T>::GetConnector() const {return *connector;}

template <typename T>
void PositionHistoricalDataService<T>::OnMessage(Position<T>& data) {PersistData("", data);}

template <typename T>
void PositionHistoricalDataService<T>::AddListener(ServiceListener<Position<T>>* listener) {listeners.push_back(listener);}

template <typename T>
const vector< ServiceListener<Position<T>>*>& PositionHistoricalDataService<T>::GetListeners() const {return listeners;}

template <typename T>
void PositionHistoricalDataService<T>::PersistData(std::string persistKey, const Position<T>& data)
{
	std::string id = data.GetProduct().GetProductId();
	auto it = dataMap.find(id);
	if (it != dataMap.end())
	{
		dataMap.erase(id);
	}
	dataMap.insert(std::pair<std::string, Position<T>>(id, data));
	connector->Publish(const_cast<Position<T>&>(data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
RiskHistoricalDataService<T>::RiskHistoricalDataService() :
	dataMap(std::map<std::string, PV01<T>>()), listeners(std::vector<ServiceListener<PV01<T>>*>()), connector(new RiskHistoricalDataServiceConnector<T>()), sectors(std::vector<BucketedSector<T>>()) {}

template <typename T>
PV01<T>& RiskHistoricalDataService<T>::GetData(std::string key) {return dataMap.at(key);}

template <typename T>
RiskHistoricalDataServiceConnector<T>& RiskHistoricalDataService<T>::GetConnector() const {return *connector;}

template <typename T>
void RiskHistoricalDataService<T>::OnMessage(PV01<T>& data) {PersistData("", data);}

template <typename T>
void RiskHistoricalDataService<T>::AddListener(ServiceListener<PV01<T>>* listener) {listeners.push_back(listener);}

template <typename T>
const vector< ServiceListener<PV01<T>>*>& RiskHistoricalDataService<T>::GetListeners() const {return listeners;}

template <typename T>
const PV01<BucketedSector<Bond>> RiskHistoricalDataService<T>::GetBucketedRisk(const BucketedSector<Bond>& sector) const
{
	double pv01;
	long quantity;
	double pv01Total = 0;
	std::string id;
	for (const auto& product : sector.GetProducts())
	{
		id = product.GetProductId();
		auto it = dataMap.find(id);
		if (it != dataMap.end())
		{// if the product is in the riskMap
			pv01 = it->second.GetPV01();
			quantity = it->second.GetQuantity();
			pv01Total += pv01 * ((double)quantity);
		}
	}
	return PV01<BucketedSector<Bond>>(sector, pv01Total, 1);
}

template <typename T>
void RiskHistoricalDataService<T>::AddBucketedSector(const BucketedSector<T>& sector) {sectors.push_back(sector);}

template <typename T>
void RiskHistoricalDataService<T>::PersistData(std::string persistKey, const PV01<T>& data)
{
	std::string id = data.GetProduct().GetProductId();
	auto it = dataMap.find(id);
	if (it != dataMap.end())
	{
		dataMap.erase(id);
	}
	dataMap.insert(std::pair<std::string, PV01<T>>(id, data));

	// if there are sectors of interest publish them as well
	if (sectors.size() > 0)
	{
		std::vector<PV01<BucketedSector<T>>> pv01s;
		for (const auto& sector : sectors)
		{
			pv01s.push_back(this->GetBucketedRisk(sector));
		}
		connector->Publish(const_cast<PV01<T>&>(data), pv01s);
	}
	else
	{
		connector->Publish(const_cast<PV01<T>&>(data));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
ExecutionHistoricalDataService<T>::ExecutionHistoricalDataService() :
	dataMap(std::map<std::string, ExecutionOrder<T>>()), listeners(std::vector<ServiceListener<ExecutionOrder<T>>*>()), connector(new ExecutionHistoricalDataServiceConnector<T>()) {}

template <typename T>
ExecutionOrder<T>& ExecutionHistoricalDataService<T>::GetData(std::string key) {return dataMap.at(key);}

template <typename T>
ExecutionHistoricalDataServiceConnector<T>& ExecutionHistoricalDataService<T>::GetConnector() const {return *connector;}

template <typename T>
void ExecutionHistoricalDataService<T>::OnMessage(ExecutionOrder<T>& data) {PersistData("", data);}

template <typename T>
void ExecutionHistoricalDataService<T>::AddListener(ServiceListener<ExecutionOrder<T>>* listener) {listeners.push_back(listener);}

template <typename T>
const vector< ServiceListener<ExecutionOrder<T>>*>& ExecutionHistoricalDataService<T>::GetListeners() const {return listeners;}

template <typename T>
void ExecutionHistoricalDataService<T>::PersistData(std::string persistKey, const ExecutionOrder<T>& data)
{
	std::string id = data.GetProduct().GetProductId();
	auto it = dataMap.find(id);
	if (it != dataMap.end())
	{
		dataMap.erase(id);
	}
	dataMap.insert(std::pair<std::string, ExecutionOrder<T>>(id, data));
	connector->Publish(const_cast<ExecutionOrder<T>&>(data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
StreamingHistoricalDataService<T>::StreamingHistoricalDataService() :
	dataMap(std::map<std::string, PriceStream<T>>()), listeners(std::vector<ServiceListener<PriceStream<T>>*>()), connector(new StreamingHistoricalDataServiceConnector<T>()) {}

template <typename T>
PriceStream<T>& StreamingHistoricalDataService<T>::GetData(std::string key) {return dataMap.at(key);}

template <typename T>
StreamingHistoricalDataServiceConnector<T>& StreamingHistoricalDataService<T>::GetConnector() const {return *connector;}

template <typename T>
void StreamingHistoricalDataService<T>::OnMessage(PriceStream<T>& data) {PersistData("", data);}

template <typename T>
void StreamingHistoricalDataService<T>::AddListener(ServiceListener<PriceStream<T>>* listener) {listeners.push_back(listener);}

template <typename T>
const vector< ServiceListener<PriceStream<T>>*>& StreamingHistoricalDataService<T>::GetListeners() const {return listeners;}

template <typename T>
void StreamingHistoricalDataService<T>::PersistData(std::string persistKey, const PriceStream<T>& data)
{
	std::string id = data.GetProduct().GetProductId();
	auto it = dataMap.find(id);
	if (it != dataMap.end())
	{
		dataMap.erase(id);
	}
	dataMap.insert(std::pair<std::string, PriceStream<T>>(id, data));
	connector->Publish(const_cast<PriceStream<T>&>(data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
InquiryHistoricalDataService<T>::InquiryHistoricalDataService() :
	dataMap(std::map<std::string, Inquiry<T>>()), listeners(std::vector<ServiceListener<Inquiry<T>>*>()), connector(new InquiryHistoricalDataServiceConnector<T>()) {}

template <typename T>
Inquiry<T>& InquiryHistoricalDataService<T>::GetData(std::string key) {return dataMap.at(key);}

template <typename T>
InquiryHistoricalDataServiceConnector<T>& InquiryHistoricalDataService<T>::GetConnector() const {return *connector;}

template <typename T>
void InquiryHistoricalDataService<T>::OnMessage(Inquiry<T>& data) {PersistData("", data);}

template <typename T>
void InquiryHistoricalDataService<T>::AddListener(ServiceListener<Inquiry<T>>* listener) {listeners.push_back(listener);}

template <typename T>
const vector< ServiceListener<Inquiry<T>>*>& InquiryHistoricalDataService<T>::GetListeners() const {return listeners;}

// Persist data to a store
template <typename T>
void InquiryHistoricalDataService<T>::PersistData(std::string persistKey, const Inquiry<T>& data)
{
	std::string id = data.GetInquiryId(); 
	auto it = dataMap.find(id);
	if (it != dataMap.end())
	{
		dataMap.erase(id);
	}
	dataMap.insert(std::pair<std::string, Inquiry<T>>(id, data));
	connector->Publish(const_cast<Inquiry<T>&>(data));
}

#endif