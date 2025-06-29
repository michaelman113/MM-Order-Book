#include "OrderBook.hpp"
#include "Order.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main() {
    OrderBook ob;
    const int numOrders = 100'000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numOrders; ++i) {
        Order o{i, 100.0, 100, (i % 2 == 0) ? Side::BUY : Side::SELL, 0};
        ob.addOrder(o);
    }

    ob.match();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Processed " << numOrders << " orders in "
              << duration_ns / 1e6 << " ms\n";
    std::cout << "Throughput: " << (numOrders * 1e9 / duration_ns) << " orders/sec\n";

    return 0;
}
