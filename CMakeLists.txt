cmake_minimum_required(VERSION 4.1)
project(High_Frequency_Trading_System)

set(CMAKE_CXX_STANDARD 20)

add_executable(High_Frequency_Trading_System
        src/main.cpp
        includes/market_snapshot.h
        includes/hft_client.h
        includes/htf_server.h

        src/hft_client.cpp
        src/hft_server.cpp
        src/main.cpp
        src/market_snapshot.cpp
        includes/order_manager.h
        src/order_manager.cpp
        includes/feed_parser.h
)
