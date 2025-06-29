#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(bookMutex);
    auto& book = (order.side == Side::BUY) ? buyOrders : sellOrders;
    book[order.price].push(order);
}

void OrderBook::cancelOrder(uint64_t orderId) {
    std::lock_guard<std::mutex> lock(bookMutex);
    // Naive cancel — to be optimized later (search all price levels)
    for (auto& [price, queue] : buyOrders) {
        std::queue<Order> temp;
        while (!queue.empty()) {
            auto o = queue.front(); queue.pop();
            if (o.id != orderId) temp.push(o);
        }
        queue = temp;
    }
    for (auto& [price, queue] : sellOrders) {
        std::queue<Order> temp;
        while (!queue.empty()) {
            auto o = queue.front(); queue.pop();
            if (o.id != orderId) temp.push(o);
        }
        queue = temp;
    }
}

void OrderBook::match() {
    std::lock_guard<std::mutex> lock(bookMutex);
    while (!buyOrders.empty() && !sellOrders.empty()) {
        auto buyIt = --buyOrders.end();
        auto sellIt = sellOrders.begin();

        if (buyIt->first < sellIt->first) break;

        auto& buyQueue = buyIt->second;
        auto& sellQueue = sellIt->second;

        auto& buyOrder = buyQueue.front();
        auto& sellOrder = sellQueue.front();

        uint32_t tradedQty = std::min(buyOrder.quantity, sellOrder.quantity);

        std::cout << "Matched Order: " << tradedQty
                  << " @ Price: " << sellOrder.price << "\n";

        if ((buyOrder.quantity -= tradedQty) == 0) buyQueue.pop();
        if ((sellOrder.quantity -= tradedQty) == 0) sellQueue.pop();

        if (buyQueue.empty()) buyOrders.erase(buyIt);
        if (sellQueue.empty()) sellOrders.erase(sellIt);
    }
}
