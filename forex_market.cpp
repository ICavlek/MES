#include <iostream>
#include <string>
#include <vector>

#include "DataBaseHandler.hpp"

std::vector<PriceData> get_ask_data_from_database(std::string currency_pair, std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	std::cout << "Loading ASK data..." << std::endl;
	DataBaseFileHandler data_base(
		currency_pair, "ASK", start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

std::vector<PriceData> get_bid_data_from_database(std::string currency_pair, std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	std::cout << "Loading BID data..." << std::endl;
	DataBaseFileHandler data_base(
		currency_pair, "BID", start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

void simulate_expert_advisor(ExpertAdvisor& expert_advisor, std::vector<PriceData> ask_data_vector, std::vector<PriceData> bid_data_vector) {
	for (size_t i = 0; i < ask_data_vector.size(); i++) {
		std::string date_time = ask_data_vector[i].date_time;
		double price_ask = ask_data_vector[i].price;
		double price_bid = bid_data_vector[i].price;

		std::cout << date_time + " ";
		expert_advisor.api_handler.update_orders(date_time, price_ask, price_bid);
		expert_advisor.handle_state_machine(price_ask, price_bid);
	}
}

void forex_market(ExpertAdvisor& expert_advisor, std::string currency_pair, std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	auto ask_data_vector = get_ask_data_from_database(currency_pair, start_date, start_time, end_date, end_time);
	auto bid_data_vector = get_bid_data_from_database(currency_pair, start_date, start_time, end_date, end_time);

	expert_advisor.initialize(ask_data_vector[0].price);
	simulate_expert_advisor(expert_advisor, ask_data_vector, bid_data_vector);
}