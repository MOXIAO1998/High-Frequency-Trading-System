//
// Created by Moxiao Li on 4/19/26.
//
#include "../includes/order_manager.h"
#include <iostream>
using namespace std;
int OrderManager::place_order(Side side, double price, int qty) {
    int id = next_id++;
    auto order = make_unique<MyOrder>();
    order->id = id;
    order->side = side;
    order->price = price;
    order->quantity = qty;

    orders[id] = std::move(order);
    return id;
}


void OrderManager::cancel(int id) {
    auto it = orders.find(id);
    if (it == orders.end()) {
        std::cout << "Order " << id << " not found\n";
        return;
    }

    it->second->status = OrderStatus::Cancelled;
    cout << "Order " << id << " cancelled\n";
    orders.erase(it);
}

void OrderManager::handle_fill(int id, int filled_qty) {
    auto it = orders.find(id);
    if (it == orders.end()) {
        return;
    }

    MyOrder* order = it->second.get();
    order->filled += filled_qty;

    if (order->filled >= order->quantity) {
        order->filled = order->quantity;
        order->status = OrderStatus::Filled;

        std::cout << "[Order] Order " << id
                  << " completed ("
                  << order->filled
                  << " / "
                  << order->quantity
                  << ") and removed\n";

        orders.erase(it);
    } else {
        order->status = OrderStatus::PartiallyFilled;

        std::cout << "[Order] Order " << id
                  << " partially filled: "
                  << order->filled
                  << " / "
                  << order->quantity
                  << "\n";
    }

}

std::string OrderManager::get_side(const Side side) {
    return (side == Side::Buy) ? "Buy" : "Sell";
}

std::string OrderManager::get_status(const OrderStatus status) {
    switch (status) {
        case OrderStatus::New:
            return "New";
        case OrderStatus::Filled:
            return "Filled";
        case OrderStatus::PartiallyFilled:
            return "PartiallyFilled";
        case OrderStatus::Cancelled:
            return "Cancelled";
    }
    return "Unknown";
}


void OrderManager::print_active_orders() const {
    if (orders.empty()) {
        std::cout << "No active orders\n";
        return;
    }

    for (const auto& [id, order_ptr] : orders) {
        cout << "Order: " << id
             << " side= " << get_side(order_ptr->side)
             << " price= " << order_ptr->price
             << " qty= " << order_ptr->quantity
             << " filled= " << order_ptr->filled
             << " status= " << get_status(order_ptr->status)
             << "\n";
    }
}


bool OrderManager::has_active_order() const {
    if (orders.empty()) {
        return false;
    }
    return true;
}
bool OrderManager::has_active_sell_order() const {
    for (const auto& [id, order_ptr] : orders) {
        if (order_ptr->side == Side::Sell) {
            return true;
        }
    }
    return false;
}
bool OrderManager::has_active_buy_order() const {
    for (const auto& [id, order_ptr] : orders) {
        if (order_ptr->side == Side::Buy) {
            return true;
        }
    }
    return false;
}