#ifndef BOND_TRADE_BOOKING_SERVICE_LISTENER_HPP
#define BOND_TRADE_BOOKING_SERVICE_LISTENER_HPP

#include "BondTradeBookingService.hpp"

#include "../executionservice.hpp"
#include "../soa.hpp"
#include "../products.hpp"

class BondTradeBookingServiceListener : public ServiceListener<ExecutionOrder<Bond>>
{
    private:
        BondTradeBookingService* service;

    public:
        BondTradeBookingServiceListener(BondTradeBookingService* _service);
        virtual void ProcessAdd(ExecutionOrder<Bond>& data); // process an add event
        virtual void ProcessRemove(ExecutionOrder<Bond>& data); // process a remove event
        virtual void ProcessUpdate(ExecutionOrder<Bond>& data); // process an update event
};

// implementation

BondTradeBookingServiceListener::BondTradeBookingServiceListener(BondTradeBookingService* _service) : service(_service) {}

void BondTradeBookingServiceListener::ProcessAdd(ExecutionOrder<Bond>& data)
{
    // create Trade from the execution order
    Bond product = data.GetProduct();
    std::string tradeId = "TRADE" + data.GetOrderId();
    OrderType orderType = data.GetOrderType();
    double price = data.GetPrice();
    std::string book = "EXECUTIONBOOK";
    long visibleQuantity = data.GetVisibleQuantity();
    long hiddenQuantity = data.GetHiddenQuantity();
    Side side = (data.GetSide() == BID ? SELL : BUY);
    Trade<Bond> trade(product, tradeId, price, book, visibleQuantity, side);
    service->BookTrade(trade);
}

void BondTradeBookingServiceListener::ProcessRemove(ExecutionOrder<Bond>& data) {}

void BondTradeBookingServiceListener::ProcessUpdate(ExecutionOrder<Bond>& data) {}

#endif