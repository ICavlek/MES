#include "MarketOrderHandler.hpp"

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

void test_all_other_orders() {
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
		test_all_other_orders();
	}
	catch (MOHBaseException& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}