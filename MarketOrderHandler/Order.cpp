#include "MarketOrderHandlerExceptions.hpp"

class Order {
public:
	Order(
		double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) :
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

	virtual void handle_success(double price) = 0;
	virtual void handle_fail(double price) = 0;

	void order_successfully_created(std::string order_type, int ticket_number) {
		std::cout << order_type + " " + std::to_string(ticket_number) + " has been successfully created" << std::endl;
		std::cout << std::endl;
	}

	void activate_order(std::string order_type, double price_ask) {
		std::cout << order_type + " order " + std::to_string(ticket_number) +
			" has been activated at price " + std::to_string(price_ask) << std::endl;
		std::cout << std::endl;
		activate();
	}

	void handle_success_base(std::string order_type, double price) {
		std::cout << "SUCCES " + order_type + " order " + std::to_string(ticket_number) + 
			" has hit T/P due to price " + std::to_string(price) << std::endl;
		std::cout << std::endl;
	}

	void handle_fail_base(std::string order_type, double price) {
		std::cout << "FAIL " + order_type + " order " + std::to_string(ticket_number) + 
			" has hit S/L due to price " + std::to_string(price) << std::endl;
		std::cout << std::endl;
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
		double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : Order(order_price, take_profit, stop_loss, volume, ticket_number) {
		if ((order_price > take_profit) || (order_price < stop_loss)) {
			throw MOHWrongOrderPrice("BUY", order_price, take_profit, stop_loss);
		}
	}

	virtual void handle_success(double price_bid) = 0;
	virtual void handle_fail(double price_bid) = 0;

	bool close_condition(double price_bid) {
		if (is_active()) {
			if (price_bid >= get_take_profit()) {
				handle_success(price_bid);
				return true;
			}
			else if (price_bid <= get_stop_loss()) {
				handle_fail(price_bid);
				return true;
			}
		}
		return false;
	}
};

class SellOrder : public Order {
public:
	SellOrder(
		double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : Order(order_price, take_profit, stop_loss, volume, ticket_number) {
		if ((order_price < take_profit) || (order_price > stop_loss)) {
			throw MOHWrongOrderPrice("SELL", order_price, take_profit, stop_loss);
		}
	}

	virtual void handle_success(double price_ask) = 0;
	virtual void handle_fail(double price_ask) = 0;

	bool close_condition(double price_ask) {
		if (is_active()) {
			if (price_ask <= get_take_profit()) {
				handle_success(price_ask);
				return true;
			}
			else if (price_ask >= get_stop_loss()) {
				handle_fail(price_ask);
				return true;
			}
		}
		return false;
	}

};

class BuyLimit : public BuyOrder {
public:
	BuyLimit(
		double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : BuyOrder(order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price > market_price) {
			throw MOHWrongPendingOrderPrice("BUY LIMIT", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created("Buy Limit", ticket_number);
	}

	void init_condition(double price_ask) {
		if (!is_active()) {
			if (price_ask <= get_order_price()) {
				activate_order("Buy Limit", price_ask);
			}
		}
	}

	void handle_success(double price_bid) {
		handle_success_base("Buy Limit", price_bid);
	}

	void handle_fail(double price_bid) {
		handle_fail_base("Buy Limit", price_bid);
	}

};

class BuyStop : public BuyOrder {
public:
	BuyStop(
		double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : BuyOrder(order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price < market_price) {
			throw MOHWrongPendingOrderPrice("BUY STOP", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created("Buy Stop", ticket_number);
	}

	void init_condition(double price_ask) {
		if (!is_active()) {
			if (price_ask >= get_order_price()) {
				activate_order("Buy Stop", price_ask);
			}
		}
	}

	void handle_success(double price_bid) {
		handle_success_base("Buy Stop", price_bid);
	}

	void handle_fail(double price_bid) {
		handle_fail_base("Buy Stop", price_bid);
	}
};

class SellLimit : public SellOrder {
public:
	SellLimit(
		double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : SellOrder(order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price < market_price) {
			throw MOHWrongPendingOrderPrice("SELL LIMIT", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created("Sell Limit", ticket_number);
	}

	void init_condition(double price_bid) {
		if (!is_active()) {
			if (price_bid >= get_order_price()) {
				activate_order("Sell Limit", price_bid);
			}
		}
	}

	void handle_success(double price_ask) {
		handle_success_base("Sell Limit", price_ask);
	}

	void handle_fail(double price_ask) {
		handle_fail_base("Sell Limit", price_ask);
	}
};

class SellStop : public SellOrder {
public:
	SellStop(
		double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : SellOrder(order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price > market_price) {
			throw MOHWrongPendingOrderPrice("SELL STOP", market_price, order_price, take_profit, stop_loss);
		}
		order_successfully_created("Sell Stop", ticket_number);
	}

	void init_condition(double price_bid) {
		if (!is_active()) {
			if (price_bid <= get_order_price()) {
				activate_order("Sell Stop", price_bid);
			}
		}
	}

	void handle_success(double price_ask) {
		handle_success_base("Sell Stop", price_ask);
	}

	void handle_fail(double price_ask) {
		handle_fail_base("Sell Stop", price_ask);
	}
};

class MarketOrderHandler {
public:
	MarketOrderHandler() : next_ticket_number(0) {}

	void create_new_buy_limit(double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		buy_limit_orders.push_back(BuyLimit(market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_buy_stop(double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		buy_stop_orders.push_back(BuyStop(market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_sell_limit(double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		sell_limit_orders.push_back(SellLimit(market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void create_new_sell_stop(double market_price, double order_price, double take_profit, double stop_loss, double volume) {
		sell_stop_orders.push_back(SellStop(market_price, order_price, take_profit, stop_loss, volume, next_ticket_number));
		++next_ticket_number;
	}

	void update_orders(double price_ask, double price_bid) {
		std::cout << "Handling Price Ask " + std::to_string(price_ask) + " and Price Bid " + std::to_string(price_bid) + "..." << std::endl;
		update_buy_orders(buy_limit_orders, price_ask, price_bid);
		update_buy_orders(buy_stop_orders, price_ask, price_bid);
		update_sell_orders(sell_limit_orders, price_ask, price_bid);
		update_sell_orders(sell_stop_orders, price_ask, price_bid);
	}

private:
	std::vector<BuyLimit> buy_limit_orders;
	std::vector<BuyStop> buy_stop_orders;
	std::vector<SellLimit> sell_limit_orders;
	std::vector<SellStop> sell_stop_orders;
	int next_ticket_number;

	template<typename OrderType>
	void update_buy_orders(OrderType& buy_orders, double price_ask, double price_bid) {
		std::vector<int> indices_to_delete;
		for (size_t i = 0; i < buy_orders.size(); i++) {
			auto& order = buy_orders[i];
			order.init_condition(price_ask);
			bool is_closed = order.close_condition(price_bid);
			if (is_closed) {
				indices_to_delete.push_back(i);
			}
		}
		delete_element(buy_orders, indices_to_delete);
	}

	template<typename OrderType>
	void update_sell_orders(OrderType& sell_orders, double price_ask, double price_bid) {
		std::vector<int> indices_to_delete;
		for (size_t i = 0; i < sell_orders.size(); i++) {
			auto& order = sell_orders[i];
			order.init_condition(price_bid);
			bool is_closed = order.close_condition(price_ask);
			if (is_closed) {
				indices_to_delete.push_back(i);
			}
		}
		delete_element(sell_orders, indices_to_delete);
	}

	template<typename OrderType>
	void delete_element(std::vector<OrderType>& vector_order, std::vector<int> indices) {
		for (auto index : indices) {
			vector_order.erase(vector_order.begin() + index);
		}
	}
};