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
		ticket_number(ticket_number) {}

private:
	double order_price;
	double take_profit;
	double stop_loss;
	double volume;
	int ticket_number;
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
};

class BuyLimit : public BuyOrder {
public:
	BuyLimit(
		double market_price, double order_price, double take_profit, double stop_loss, double volume, int ticket_number
	) : BuyOrder(order_price, take_profit, stop_loss, volume, ticket_number) {
		if (order_price > market_price) {
			throw MOHWrongPendingOrderPrice("BUY LIMIT", market_price, order_price, take_profit, stop_loss);
		}
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
	}
};