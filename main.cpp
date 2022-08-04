#include <iostream>
#include <string>
#include <vector>

#include "DataBaseHandler.hpp"
#include "MarketOrderHandler.hpp"

std::vector<PriceData> get_data(std::string path, std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	DataBaseFileHandler data_base(
		path, start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

void test_get_data_from_database() {
	std::string path = "D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\GBPUSD_1_M_ASK_2022.csv";
	std::string start_date = "31.07.2022";
	std::string start_time = "23:57:00.000";
	std::string end_date = "31.07.2022";
	std::string end_time = "23:59:00.000";

	std::vector<PriceData> data = get_data(path, start_date, start_time, end_date, end_time);

	for (auto& element : data) {
		std::cout << element.date_time + " ";
		std::cout << element.price << std::endl;
	}
}

void test_buy_limit() {
	MarketOrderHandler market;
	std::string date_time = "01.01.2022. 01:14:20:000";
	std::ofstream trade_report("D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\Trade_Report_GBPUSD.csv");

	market.create_new_buy_limit(date_time, 2, 1, 2, 0, 1);
	market.update_orders(2.1, 2, trade_report);
	market.update_orders(1.1, 1.05, trade_report);
	market.update_orders(0.95, 0.94, trade_report);
	market.update_orders(0.1, -0.1, trade_report);
}

void test_buy_stop() {
	MarketOrderHandler market;
	std::string date_time = "01.01.2022. 01:14:20:000";
	std::ofstream trade_report("D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\Trade_Report_GBPUSD.csv");

	market.create_new_buy_stop(date_time, 2, 3, 4, 1, 1);
	market.update_orders(2.1, 2, trade_report);
	market.update_orders(3.1, 3.05, trade_report);
	market.update_orders(1.95, 1.94, trade_report);
	market.update_orders(0.1, -0.1, trade_report);
	market.create_new_sell_limit(date_time, 0.1, 1, 0.5, 2, 1);
	market.update_orders(0.5, 0.4, trade_report);
	market.update_orders(1.2, 1.1, trade_report);
	market.update_orders(0.4, 0.5, trade_report);
	market.create_new_sell_stop(date_time, 0.5, 0.3, 0.2, 0.6, 1);
	market.update_orders(0.35, 0.3, trade_report);
	market.update_orders(0.5, 0.4, trade_report);
	market.update_orders(0.7, 0.6, trade_report);
}

void test_orders() {
	try {
		//test_buy_limit();
		test_buy_stop();
	}
	catch (MOHBaseException& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

int main(int argc, char* argv[], char* envp[]) {
	//test_get_data_from_database();
	test_orders();
}