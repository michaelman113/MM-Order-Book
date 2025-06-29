cmake_minimum_required(VERSION 3.12)
project(order_book_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(order_book
    src/main.cpp
    src/OrderBook.cpp
)
