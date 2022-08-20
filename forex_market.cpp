#include <iostream>
#include <string>
#include <vector>

#include "DataBaseHandler.hpp"

class ForexMarket {
public:
	ForexMarket(std::string currency_pair, std::string start_date, std::string start_time, std::string end_date, std::string end_time) :
		currency_pair(currency_pair),
		start_date(start_date),
		start_time(start_time),
		end_date(end_date),
		end_time(end_time) {
		ask_data = get_data_from_database("ASK");
		bid_data = get_data_from_database("BID");
	}

	void run_strategy(ExpertAdvisor& expert_advisor) {
		expert_advisor.initialize(ask_data[0].price);
		simulate_expert_advisor(expert_advisor);
	}

private:
	std::string currency_pair;
	std::string start_date;
	std::string start_time;
	std::string end_date;
	std::string end_time;
	std::vector<PriceData> ask_data;
	std::vector<PriceData> bid_data;

	std::vector<PriceData> get_data_from_database(std::string type) {
		std::cout << "Loading " + type + " data..." << std::endl;
		DataBaseFileHandler data_base(
			currency_pair, type, start_date, start_time, end_date, end_time
		);
		return data_base.get_data_vector();
	}

	void simulate_expert_advisor(ExpertAdvisor& expert_advisor) {
		for (size_t i = 0; i < ask_data.size(); i++) {
			std::string date_time = ask_data[i].date_time;
			double price_ask = ask_data[i].price;
			double price_bid = bid_data[i].price;

			std::cout << date_time + " ";
			expert_advisor.api_handler.update_orders(date_time, price_ask, price_bid);
			expert_advisor.handle_state_machine(price_ask, price_bid);
		}
	}
};