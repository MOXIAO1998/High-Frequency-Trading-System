//
// Created by Moxiao Li on 4/19/26.
//
#include "../includes/market_snapshot.h"
#include <iostream>
using namespace std;

void MarketSnapshot::update_bid(double price, int qty) {
    if (qty == 0) {
        bids.erase(price);
        return;
    }
    auto it = bids.find(price); // iterator of map with price
    if (it == bids.end()) {
        bids[price] = make_unique<PriceLevel>(price, qty);
    }else {
        it->second->quantity = qty;
    }


};

void MarketSnapshot::update_ask(double price, int qty) {
    if (qty == 0) {
        asks.erase(price);
        return;
    }
    auto it = asks.find(price); // iterator of map with price
    if (it == asks.end()) {
        asks[price] = make_unique<PriceLevel>(price, qty);
    }else {
        it->second->quantity = qty;
    }
};

const PriceLevel* MarketSnapshot::get_best_bid() const {
    if (bids.empty()) {
        return nullptr;
    }else {
        return bids.begin()->second.get();
    }
}

const PriceLevel* MarketSnapshot::get_best_ask() const {
    if (asks.empty()) {
        return nullptr;
    }else {
        return asks.begin()->second.get();
    }

}


void MarketSnapshot::print_book() const {
    const PriceLevel* best_bid = get_best_bid();
    const PriceLevel* best_ask = get_best_ask();

    if (best_bid) {
        std::cout << "Best Bid: " << best_bid->price
                  << " qty= " << best_bid->quantity << "\n";
    } else {
        std::cout << "Best Bid: none\n";
    }

    if (best_ask) {
        std::cout << "Best Ask: " << best_ask->price
                  << " qty= " << best_ask->quantity << "\n";
    } else {
        std::cout << "Best Ask: none\n";
    }
}
