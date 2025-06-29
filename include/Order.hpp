#pragma once
#include <cstdint>

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    double price;
    uint32_t quantity;
    Side side;
    uint64_t timestamp_ns;
};
