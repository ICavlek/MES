#define MES_ENVIRONMENT

#include <iostream>
#include <string>
#include <vector>

#include "DataBaseHandler.hpp"
#include "APIHandler.hpp"
#include "ExpertAdvisor/ExpertAdvisor.mqh"

std::vector<PriceData> get_ask_data_from_database(std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	std::string path = "D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\GBPUSD_1_M_ASK_2022_Reduced.csv";
	std::cout << "Loading ASK data..." << std::endl;
	DataBaseFileHandler data_base(
		path, start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

std::vector<PriceData> get_bid_data_from_database(std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	std::string path = "D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\GBPUSD_1_M_BID_2022_Reduced.csv";
	std::cout << "Loading BID data..." << std::endl;
	DataBaseFileHandler data_base(
		path, start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

void forex_market() {
	std::string start_date = "28.07.2022";
	std::string start_time = "00:00:00.000";
	std::string end_date = "28.07.2022";
	std::string end_time = "23:59:00.000";

	auto ask_data_vector = get_ask_data_from_database(start_date, start_time, end_date, end_time);
	auto bid_data_vector = get_bid_data_from_database(start_date, start_time, end_date, end_time);

	if (ask_data_vector.size() != bid_data_vector.size()) {
		std::cout << "ASK and BID data are not of the same size";
	}

	STATES current_state = NONE_NONE;
	double discrete_level = 0.002;
	double init_balance = 2000;
	int percentage_of_balance_to_use = 1;
	bool use_max_balance = false;
	double current_price_ask = ask_data_vector[0].price;
	ExpertAdvisor expert_advisor(current_state, discrete_level, init_balance, percentage_of_balance_to_use, use_max_balance, current_price_ask);

	for (size_t i = 0; i < ask_data_vector.size(); i++) {
		std::string date_time = ask_data_vector[i].date_time;
		double price_ask = ask_data_vector[i].price;
		double price_bid = bid_data_vector[i].price;
		
		expert_advisor.api_handler.update_orders(date_time, price_ask, price_bid);
		expert_advisor.handle_state_machine(price_ask, price_bid);
	}
}

int main(int argc, char* argv[], char* envp[]) {
	try {
		forex_market();
	}
	catch (MOHBaseException& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}