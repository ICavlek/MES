#include "MarketOrderHandlerExceptions.hpp"

class Order {
public:
	Order(
		std::string date_time, std::string order_type, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) :
		date_time(date_time),
		order_type(order_type),
		order_price(order_price),
		take_profit(take_profit),
		stop_loss(stop_loss),
		volume(volume),
		ticket_number(ticket_number),
		active(false) {
		std::cout << "----------Creating Order---------" << std::endl;
		std::cout << "Order Price: " + std::to_string(order_price) << std::endl;
		std::cout << "T/P: " + std::to_string(take_profit) << std::endl;
		std::cout << "S/L: " + std::to_string(stop_loss) << std::endl;
		std::cout << "----------Order Created----------" << std::endl;
	}

	void order_info() {
		std::cout << order_type + " " + date_time + " Open Price: " + std::to_string(order_price) +
			" T/P: " + std::to_string(take_profit) + " S/L: " + std::to_string(stop_loss) << std::endl;
	}

	void order_successfully_created(int ticket_number) {
		std::cout << order_type + " " + std::to_string(ticket_number) + " has been successfully created" << std::endl;
		std::cout << std::endl;
	}

	void activate_order(double price_ask) {
		std::cout << order_type + " order " + std::to_string(ticket_number) +
			" has been activated at price " + std::to_string(price_ask) << std::endl;
		std::cout << std::endl;
		activate();
	}

	void handle_success(double price, double& account_balance, std::ofstream& trade_report) {
		std::cout << "SUCCES " + order_type + " order " + std::to_string(ticket_number) + 
			" has hit T/P due to price " + std::to_string(price) << std::endl;
		std::cout << std::endl;
		double profit = std::abs(order_price - take_profit);
		double swap = 0;
		account_balance += profit * volume * 100000;
		trade_report << date_time + "," + order_type + "," + "SUCCESS" + "," +
			std::to_string(order_price) + "," + std::to_string(stop_loss) +
			"," + std::to_string(volume) + "," + std::to_string(profit) + "," + 
			std::to_string(swap) + "," + std::to_string(account_balance) << std::endl;
	}

	void handle_fail(double price, double& account_balance, std::ofstream& trade_report) {
		std::cout << "FAIL " + order_type + " order " + std::to_string(ticket_number) + 
			" has hit S/L due to price " + std::to_string(price) << std::endl;
		std::cout << std::endl;
		double profit = std::abs(order_price - take_profit);
		double swap = 0;
		account_balance -= profit * volume * 100000;
		trade_report << date_time + "," + order_type + "," + "FAIL" + "," +
			std::to_string(order_price) + "," + std::to_string(stop_loss) +
			"," + std::to_string(volume) + "," + std::to_string(profit) + "," +
			"," + std::to_string(swap) + "," + std::to_string(account_balance) << std::endl;
	}

	bool is_active() {
		return active;
	}

	void activate() {
		active = true;
	}

	double get_order_price() {
		return order_price;
	}

	double get_take_profit() {
		return take_profit;
	}

	double get_stop_loss() {
		return stop_loss;
	}

	int get_ticket_number() {
		return ticket_number;
	}

private:
	std::string date_time;
	std::string order_type;
	double order_price;
	double take_profit;
	double stop_loss;
	double volume;
	int ticket_number;
	bool active;
};

class BuyOrder : public Order {
public:
	BuyOrder(
		std::string date_time, std::string order_type, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : Order(date_time, order_type, order_price, take_profit, stop_loss, volume, ticket_number) {
		if ((order_price > take_profit) || (order_price < stop_loss)) {
			throw MOHWrongOrderPrice("BUY", order_price, take_profit, stop_loss);
		}
	}

	bool close_condition(double price_bid, double& account_balance, std::ofstream& trade_report) {
		if (is_active()) {
			if (price_bid >= get_take_profit()) {
				handle_success(price_bid, account_balance, trade_report);
				return true;
			}
			else if (price_bid <= get_stop_loss()) {
				handle_fail(price_bid, account_balance,  trade_report);
				return true;
			}
		}
		return false;
	}
};

class SellOrder : public Order {
public:
	SellOrder(
		std::string date_time, std::string order_type, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : Order(date_time, order_type, order_price, take_profit, stop_loss, volume, ticket_number) {
		if ((order_price < take_profit) || (order_price > stop_loss)) {
			throw MOHWrongOrderPrice("SELL", order_price, take_profit, stop_loss);
		}
	}

	bool close_condition(double price_ask, double& account_balance, std::ofstream& trade_report) {
		if (is_active()) {
			if (price_ask <= get_take_profit()) {
				handle_success(price_ask, account_balance, trade_report);
				return true;
			}
			else if (price_ask >= get_stop_loss()) {
				handle_fail(price_ask, account_balance, trade_report);
				return true;
			}
		}
		return false;
	}

};

class BuyLimit : public BuyOrder {
public:
	BuyLimit(
		std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : BuyOrder(date_time, "BUY LIMIT", order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price > market_price) {
			throw MOHWrongPendingOrderPrice("BUY LIMIT", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created(ticket_number);
	}

	void init_condition(double price_ask) {
		if (!is_active()) {
			if (price_ask <= get_order_price()) {
				activate_order(price_ask);
			}
		}
	}
};

class BuyStop : public BuyOrder {
public:
	BuyStop(
		std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : BuyOrder(date_time, "BUY STOP", order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price < market_price) {
			throw MOHWrongPendingOrderPrice("BUY STOP", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created(ticket_number);
	}

	void init_condition(double price_ask) {
		if (!is_active()) {
			if (price_ask >= get_order_price()) {
				activate_order(price_ask);
			}
		}
	}
};

class SellLimit : public SellOrder {
public:
	SellLimit(
		std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : SellOrder(date_time, "SELL LIMIT", order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price < market_price) {
			throw MOHWrongPendingOrderPrice("SELL LIMIT", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created(ticket_number);
	}

	void init_condition(double price_bid) {
		if (!is_active()) {
			if (price_bid >= get_order_price()) {
				activate_order(price_bid);
			}
		}
	}
};

class SellStop : public SellOrder {
public:
	SellStop(
		std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : SellOrder(date_time, "SELL STOP", order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price > market_price) {
			throw MOHWrongPendingOrderPrice("SELL STOP", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created(ticket_number);
	}

	void init_condition(double price_bid) {
		if (!is_active()) {
			if (price_bid <= get_order_price()) {
				activate_order(price_bid);
			}
		}
	}
};

class MarketOrderHandler {
public:
	MarketOrderHandler() : next_ticket_number(0) {}

	void create_new_buy_limit(std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		buy_limit_orders.push_back(BuyLimit(date_time, market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_buy_stop(std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		buy_stop_orders.push_back(BuyStop(date_time, market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_sell_limit(std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		sell_limit_orders.push_back(SellLimit(date_time, market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_sell_stop(std::string date_time, double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		sell_stop_orders.push_back(SellStop(date_time, market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void update_orders(double price_ask, double price_bid, double& account_balance, std::ofstream& trade_report) {
		std::cout << "Handling Price Ask " + std::to_string(price_ask) + " and Price Bid " + std::to_string(price_bid) + "..." << std::endl;
		update_buy_orders(buy_limit_orders, price_ask, price_bid, account_balance, trade_report);
		update_buy_orders(buy_stop_orders, price_ask, price_bid, account_balance, trade_report);
		update_sell_orders(sell_limit_orders, price_ask, price_bid, account_balance, trade_report);
		update_sell_orders(sell_stop_orders, price_ask, price_bid, account_balance, trade_report);
	}

	void delete_all_buy_limit_orders() {
		buy_limit_orders.clear();
	}

	void delete_all_buy_stop_orders() {
		buy_stop_orders.clear();
	}

	void delete_all_sell_limit_orders() {
		sell_limit_orders.clear();
	}

	void delete_all_sell_stop_orders() {
		sell_stop_orders.clear();
	}

private:
	std::vector<BuyLimit> buy_limit_orders;
	std::vector<BuyStop> buy_stop_orders;
	std::vector<SellLimit> sell_limit_orders;
	std::vector<SellStop> sell_stop_orders;
	int next_ticket_number;

	template<typename OrderType>
	void update_buy_orders(OrderType& buy_orders, double price_ask, double price_bid, double& account_balance, std::ofstream& trade_report) {
		std::vector<int> indices_to_delete;
		for (size_t i = 0; i < buy_orders.size(); i++) {
			buy_orders[i].order_info();
			auto& order = buy_orders[i];
			order.init_condition(price_ask);
			bool is_closed = order.close_condition(price_bid, account_balance, trade_report);
			if (is_closed) {
				indices_to_delete.push_back(i);
			}
		}
		delete_element(buy_orders, indices_to_delete);
	}

	template<typename OrderType>
	void update_sell_orders(OrderType& sell_orders, double price_ask, double price_bid, double& account_balance, std::ofstream& trade_report) {
		std::vector<int> indices_to_delete;
		for (size_t i = 0; i < sell_orders.size(); i++) {
			sell_orders[i].order_info();
			auto& order = sell_orders[i];
			order.init_condition(price_bid);
			bool is_closed = order.close_condition(price_ask, account_balance, trade_report);
			if (is_closed) {
				indices_to_delete.push_back(i);
			}
		}
		delete_element(sell_orders, indices_to_delete);
	}

	template<typename OrderType>
	void delete_element(std::vector<OrderType>& vector_order, std::vector<int> indices) {
		if (indices.size()) {
			for (int i = indices.size() - 1; i >= 0; i--) {
				vector_order.erase(vector_order.begin() + indices[i]);
			}
		}
	}
};