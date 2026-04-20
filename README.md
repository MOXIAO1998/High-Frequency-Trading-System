# High-Frequency-Trading-System

## 1. Overview of System Architecture

This project implements a small event-driven trading system core in C++. It is not a market simulator and not an exchange. Its purpose is to maintain a local view of market data, track submitted orders, and react to feed updates through a simple trading strategy.

The system is organized into the following components:

### MarketSnapshot
`MarketSnapshot` maintains the internal local order book.

Its responsibilities are:
- store bid price levels
- store ask price levels
- update price levels when new market data arrives
- remove a price level when its quantity becomes zero
- provide access to the current best bid and best ask

Bid and ask levels are stored in maps, so the program can efficiently access the top of book.

### OrderManager
`OrderManager` tracks the system’s own orders.

Its responsibilities are:
- assign unique order IDs
- place orders
- process execution fills
- track order status
- remove orders when they are fully filled or canceled

Each active order is stored in a map and is managed as a dynamically allocated object owned by a smart pointer.

### feed_parser
`feed_parser.h` reads `sample_feed.txt` and converts each line into a structured `FeedEvent`.

Supported event types are:
- `BID`
- `ASK`
- `EXECUTION`

This parser acts as the input layer of the system.

### main.cpp
`main.cpp` is the strategy execution loop.

Its responsibilities are:
- read each feed event
- update the `MarketSnapshot`
- evaluate the trading condition
- place orders through `OrderManager`
- process execution fills

So the overall system flow is:

`sample_feed.txt` → `feed_parser` → `MarketSnapshot` / `OrderManager` → strategy logic in `main.cpp` 

---

## 2. Explanation of How Memory Is Managed Safely

Memory in this project is managed using modern C++ RAII principles and smart pointers. The goal is to avoid manual memory management and let object lifetimes be handled automatically. This matches the project requirement that memory management must be explicit, responsible, and modern.

### Price Levels
Each `PriceLevel` is stored in a `std::unique_ptr` inside the bid/ask containers.

This means:
- the container owns the price level
- when a price level is removed from the map, its memory is automatically freed
- there is no need to manually call `delete`

### Orders
Each `MyOrder` is stored in a `std::unique_ptr` inside the order map.

This means:
- `OrderManager` owns each active order
- when an order is fully filled or canceled and removed from the map, its memory is automatically released
- there is no manual deallocation logic

### Why this is safe
This design avoids:
- memory leaks from forgotten `delete`
- double-delete errors
- ownership confusion
- manual lifetime bugs

The program follows these rules:
- use RAII to manage resource lifetimes
- store heap-allocated objects in `std::unique_ptr`
- avoid manual `new` and `delete` in normal project logic
- remove inactive objects from containers when they are no longer needed

Raw pointers such as the return value of `get_best_bid()` or `get_best_ask()` are only used as temporary non-owning observer pointers. Ownership always remains with the `std::unique_ptr` inside the containers.

---

## 3. Instructions on How to Compile and Run the Code
### Compile with g++
```bash
g++ -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer \
src/main.cpp src/market_snapshot.cpp src/order_manager.cpp \
-o trading_system
```

### Compile with clang
```bash
clang++ -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer \
src/main.cpp src/market_snapshot.cpp src/order_manager.cpp \
-o trading_system
```
### Run 
```bash
./trading_system
```
If `sample_feed.txt` is located in the data folder, make sure the file path in main.cpp matches the actual project structure, for example:
```bash
load_feed("data/sample_feed.txt")
```


### Run with log generation
```bash
./trading_system > output.log        
```


## 4. How to Verify Correctness and Memory Safety
### Verify correctness

Correctness can be checked by running the program on `sample_feed.txt` and comparing the output with the expected behavior described in the project requirements.

You should verify the following:

#### Market snapshot correctness
- BID and ASK updates are reflected correctly
- a price level is removed when quantity becomes zero
- the best bid and best ask are always correct after each event

Example:

- `BID 100.10 300` should create a best bid of `100.10 x 300`
- `BID 100.10 0` should remove that bid level
- `ASK 100.25 200` should update the best ask if appropriate

#### Order manager correctness
- each order receives a unique ID
- partial fills update the order correctly
- full fills remove the order from the active order container
- canceled orders are removed correctly if cancellation is implemented

Example:

- place an order of size `50`
- execution fill of `10` should make it partially filled
- execution fill of `40` should complete and remove the order

#### Strategy loop correctness
- the strategy reacts to BID and ASK events
- orders are placed when the trading rule is satisfied
- execution events are correctly forwarded to `OrderManager`

A simple way to verify correctness is to compare the terminal output or output.log against the expected sample behavior.

### Verify memory safety

Memory safety should be checked with runtime tools, as required by the project. The recommended tools are AddressSanitizer and Valgrind.

#### AddressSanitizer

Compile with AddressSanitizer enabled shown in above:

Using g++
```bash
g++ -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer \
src/main.cpp src/market_snapshot.cpp src/order_manager.cpp \
-o trading_system
```

Using clang++
```bash
clang++ -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer \
src/main.cpp src/market_snapshot.cpp src/order_manager.cpp \
-o trading_system
```

A clean run should finish without AddressSanitizer errors such as:

- heap-use-after-free
- heap-buffer-overflow
- double-free


Because this project uses RAII and `std::unique_ptr` to manage dynamic objects, inactive price levels and completed orders are automatically deallocated when erased from their containers. This is the main reason the design is memory-safe.