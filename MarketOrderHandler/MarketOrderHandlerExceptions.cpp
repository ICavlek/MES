#include <exception>

class MOHBaseException : public std::exception {
public:
	MOHBaseException(std::string order_type, double open_price, double take_profit, double stop_loss) :
		order_type(order_type),
		open_price(open_price),
		take_profit(take_profit),
		stop_loss(stop_loss) {}

	const char* what() const throw () {
		return message.c_str();
	}

protected:
	std::string order_type;
	double open_price;
	double take_profit;
	double stop_loss;
	std::string message;
};

class MOHWrongOrderPrice : public MOHBaseException {
public:
	MOHWrongOrderPrice(std::string order_type, double open_price, double take_profit, double stop_loss) : 
		MOHBaseException(
			order_type, open_price, take_profit, stop_loss
		) {
		message =
			order_type + ": Wrong order price " + std::to_string(open_price) +
			" for T/P " + std::to_string(take_profit) +
			" and S/L " + std::to_string(stop_loss);
	}
	
};

class MOHWrongPendingOrderPrice : public MOHBaseException {
public:
	MOHWrongPendingOrderPrice(std::string order_type, double market_price, double open_price, double take_profit, double stop_loss) :
		MOHBaseException(
			order_type, open_price, take_profit, stop_loss
		) {
		message =
			order_type + ": Wrong position for open price " + std::to_string(open_price) +
			" for T/P " + std::to_string(take_profit) +
			" and S/L " + std::to_string(stop_loss) + 
			" because market price is " + std::to_string(market_price);
	}
};