//
// Created by Moxiao Li on 4/19/26.
//

#ifndef HIGH_FREQUENCY_TRADING_SYSTEM_MARKET_SNAPSHOT_H
#define HIGH_FREQUENCY_TRADING_SYSTEM_MARKET_SNAPSHOT_H
#include <map>

struct PriceLevel {
    double price;
    int quantity;

    PriceLevel(double p, int q) : price(p), quantity(q) {}
};

class MarketSnapshot {
    private:
        std::map<double, std::unique_ptr<PriceLevel>, std::greater<double>> bids; // sorted descending: high to low
        std::map<double, std::unique_ptr<PriceLevel>> asks; // sorted ascending: low to high

    public:
        void update_bid(double price, int qty);
        void update_ask(double price, int qty);
        const PriceLevel* get_best_bid() const;
        const PriceLevel* get_best_ask() const;
        void print_book() const;
};


#endif //HIGH_FREQUENCY_TRADING_SYSTEM_MARKET_SNAPSHOT_H