#include <iostream>
#include <string>
#include <vector>

#include "DataBaseHandler.hpp"

std::vector<PriceData> get_data(std::string path, std::string start_date, std::string start_time, std::string end_date, std::string end_time) {
	DataBaseFileHandler data_base(
		path, start_date, start_time, end_date, end_time
	);
	return data_base.get_data_vector();
}

int main(int argc, char* argv[], char* envp[]) {
	std::string path = "D:\\Ivan\\Trading\\Historical_Data\\GBPUSD\\GBPUSD_1_M_ASK_2022.csv";
	std::string start_date = "31.07.2022";
	std::string start_time = "23:57:00.000";
	std::string end_date = "31.07.2022";
	std::string end_time = "23:59:00.000";
	
	std::vector<PriceData> data = get_data(path, start_date, start_time, end_date, end_time);

	for (auto& element : data) {
		std::cout << element.date +  " ";
		std::cout << element.time + " ";
		std::cout << element.price << std::endl;
	}

}