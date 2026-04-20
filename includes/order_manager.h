//
// Created by Moxiao Li on 4/19/26.
//

#ifndef HIGH_FREQUENCY_TRADING_SYSTEM_ORDER_MANAGER_H
#define HIGH_FREQUENCY_TRADING_SYSTEM_ORDER_MANAGER_H
#include <map>

enum class OrderStatus { New, Filled, PartiallyFilled, Cancelled };
enum class Side { Buy, Sell };

struct MyOrder {
    int id;
    Side side;
    double price;
    int quantity;
    int filled = 0;
    OrderStatus status = OrderStatus::New;
};

class OrderManager {
    private:
        std::map<int, std::unique_ptr<MyOrder>> orders;
        int next_id = 1;

    public:
        int place_order(Side side, double price, int qty);
        void cancel(int id);
        void handle_fill(int id, int filled_qty);

        static std::string get_side(const Side side);
        static std::string get_status(const OrderStatus status);

        void print_active_orders() const;

        bool has_active_order() const;
        bool has_active_sell_order() const;
        bool has_active_buy_order() const;

};


#endif //HIGH_FREQUENCY_TRADING_SYSTEM_ORDER_MANAGER_H