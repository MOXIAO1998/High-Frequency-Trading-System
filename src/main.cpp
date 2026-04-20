#include <iostream>
#include "../includes/market_snapshot.h"
#include "../includes/order_manager.h"
#include "../includes/feed_parser.h"

bool should_trade_sell(const MarketSnapshot& snapshot) {
    const PriceLevel* best_bid = snapshot.get_best_bid();
    if (!best_bid) {
        return false;
    }

    if (best_bid->price > 1000) {
        return true;
    }

    return false;
}

bool should_trade_buy(const MarketSnapshot& snapshot) {
    const PriceLevel* best_ask = snapshot.get_best_ask();
    if (!best_ask) {
        return false;
    }

    if (best_ask->price <101) {
        return true;
    }

    return false;
}


int main() {
    MarketSnapshot snapshot;
    OrderManager om;
    auto feed = load_feed("../sample_feed.txt");

    for (const auto& update : feed) {
        if (update.type == FeedType::BID) {
            const PriceLevel* old_best_bid = snapshot.get_best_bid();
            double old_bid_price = old_best_bid ? old_best_bid->price : -1.0;

            snapshot.update_bid(update.price, update.quantity);

            if (update.quantity == 0 && old_best_bid && old_bid_price == update.price) {
                std::cout << std::fixed << std::setprecision(2) << "[Market] Best Bid: " << update.price << " removed\n";
            } else {
                const PriceLevel* best_bid = snapshot.get_best_bid();
                if (best_bid) {
                    std::cout << std::fixed << std::setprecision(2) << "[Market] Best Bid: "
                              << best_bid->price
                              << " x "
                              << best_bid->quantity
                              << "\n";
                } else {
                    std::cout << "[Market] Best Bid: none\n";
                }
            }
        } else if (update.type == FeedType::ASK) {
            const PriceLevel* old_best_ask = snapshot.get_best_ask();
            double old_ask_price = old_best_ask ? old_best_ask->price : -1.0;

            snapshot.update_ask(update.price, update.quantity);

            const PriceLevel* best_ask = snapshot.get_best_ask();

            if (update.quantity == 0 && old_best_ask && old_ask_price == update.price) {
                std::cout << std::fixed << std::setprecision(2) << "[Market] Best Ask: " << update.price << " removed\n";
            } else if (best_ask) {
                if (!old_best_ask) {
                    std::cout << std::fixed << std::setprecision(2) << "[Market] Best Ask: "
                              << best_ask->price
                              << " x "
                              << best_ask->quantity
                              << "\n";
                } else if (best_ask->price != old_ask_price) {
                    std::cout << "[Market] New Ask: "
                              << best_ask->price
                              << " x "
                              << best_ask->quantity
                              << "\n";
                } else {
                    std::cout << "[Market] Best Ask: "
                              << best_ask->price
                              << " x "
                              << best_ask->quantity
                              << "\n";
                }
            } else {
                std::cout << "[Market] Best Ask: none\n";
            }
        } else if (update.type == FeedType::EXECUTION) {
            std::cout << "[Execution] Order "
                      << update.order_id
                      << " filled: "
                      << update.quantity
                      << "\n";

            om.handle_fill(update.order_id, update.quantity);
            continue;
        }

        if ((update.type == FeedType::BID || update.type == FeedType::ASK)
            && should_trade_sell(snapshot)) {
            const PriceLevel* best_ask = snapshot.get_best_ask();
            if (best_ask) {
                int id = om.place_order(Side::Sell, best_ask->price, 50);
                std::cout << "[Strategy] Placing SELL order at "
                          << best_ask->price
                          << " x 50 (ID = "
                          << id
                          << ")\n";
            }
        }


    }


}