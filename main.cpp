#include "forex_market.hpp"

int main() {
	std::string currency_pair = "GBPUSD";
	std::string start_date = "28.01.2022";
	std::string start_time = "00:00:00.000";
	std::string end_date = "02.02.2022";
	std::string end_time = "23:59:00.000";

	try {
		forex_market(currency_pair, start_date, start_time, end_date, end_time);
	}
	catch (MOHBaseException& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}