#pragma once
#include "Order.hpp"
#include <map>
#include <queue>
#include <mutex>

class OrderBook {
public:
    void addOrder(const Order& order);
    void cancelOrder(uint64_t orderId);
    void match();

private:
    std::map<double, std::queue<Order>> buyOrders;
    std::map<double, std::queue<Order>> sellOrders;
    std::mutex bookMutex; // replace w/ lock-free later
};
