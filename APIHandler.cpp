#include <iostream>
#include <string>
#include <vector>

#include "MarketOrderHandler/MarketOrderHandler.hpp"

class APIHandler {
public:
    APIHandler(double init_balance) :
        market_order_handler(),
        balance(init_balance),
        current_date_time(),
        current_price_ask(),
        current_price_bid() {}

    void update_orders(std::string date_time, double price_ask, double price_bid, std::ofstream& trade_report) {
        current_date_time = date_time;
        current_price_ask = price_ask;
        current_price_bid = price_bid;
        market_order_handler.update_orders(price_ask, price_bid, balance, trade_report);
    }

    double normalize_double(double value, int num_decimal) {
        return std::ceil(value * pow(10.0, num_decimal)) / pow(10.0, num_decimal);
    }

    double account_balance() {
        return balance;
    }

    void create_new_buy_stop(double volume, double open_price, double take_profit, double stop_loss) {
        market_order_handler.create_new_buy_stop(
            current_date_time, current_price_ask, open_price, 
            take_profit, stop_loss, volume
        );
    }

    void create_new_buy_limit(double volume, double open_price, double take_profit, double stop_loss) {
        market_order_handler.create_new_buy_limit(
            current_date_time, current_price_ask, open_price,
            take_profit, stop_loss, volume
        );
    }

    void create_new_sell_stop(double volume, double open_price, double take_profit, double stop_loss) {
        market_order_handler.create_new_sell_stop(
            current_date_time, current_price_bid, open_price,
            take_profit, stop_loss, volume
        );
    }

    void create_new_sell_limit(double volume, double open_price, double take_profit, double stop_loss) {
        market_order_handler.create_new_sell_limit(
            current_date_time, current_price_bid, open_price,
            take_profit, stop_loss, volume
        );
    }

    void delete_all_buy_stop_orders() {
        market_order_handler.delete_all_buy_stop_orders();
    }

    void delete_all_buy_limit_orders() {
        market_order_handler.delete_all_buy_limit_orders();
    }

    void delete_all_sell_stop_orders() {
        market_order_handler.delete_all_sell_stop_orders();
    }

    void delete_all_sell_limit_orders() {
        market_order_handler.delete_all_sell_limit_orders();
    }
private:
    MarketOrderHandler market_order_handler;
    double balance;
    std::string current_date_time;
    double current_price_ask;
    double current_price_bid;
};