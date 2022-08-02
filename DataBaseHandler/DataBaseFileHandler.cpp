#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "PriceData.hpp"

class DataBaseFileHandler {
public:
	DataBaseFileHandler(
		std::string path, std::string start_date, std::string start_time, std::string end_date, std::string end_time
	) :
		path(path),
		start_date(start_date),
		start_time(start_time),
		end_date(end_date),
		end_time(end_time) {
		fill_data_vector();
	}

	std::vector<PriceData> get_data_vector() {
		return data_vector;
	}

private:
	std::string path;
	std::string start_date;
	std::string start_time;
	std::string end_date;
	std::string end_time;
	std::vector<PriceData> data_vector;

	void fill_data_vector() {
		std::string line;
		std::ifstream myfile(path);
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				if (add_data(line)) {
					break;
				}
			}
			myfile.close();
		}
		else {
			std::cout << "Unable to open file";
		}
	}

	int add_data(std::string line) {
		std::vector<std::string> data = split_string_by_delimiter(line, ",");
		if (data_vector.empty()) {
			if (data[0] == start_date and data[1] == start_time) {
				add_single_data(data);
			}
		}
		else {
			add_single_data(data);
			if (data[0] == end_date and data[1] == end_time) {
				return 1;
			}
		}
		return 0;
	}

	std::vector<std::string> split_string_by_delimiter(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}

	void add_single_data(std::vector<std::string> data) {
		std::string date = data[0];
		std::string time = data[1];
		double open_price = atof(data[2].c_str());
		double high_price = atof(data[3].c_str());
		double low_price = atof(data[4].c_str());
		double close_price = atof(data[5].c_str());
		double volume = atof(data[6].c_str());

		PriceData open_price_data(
			date, time, open_price, volume
		);
		data_vector.push_back(open_price_data);

		std::string high_price_time = time.substr(0, time.size() - 6) + "20.000";
		PriceData high_price_data(
			date, high_price_time, high_price, volume
		);
		data_vector.push_back(high_price_data);

		std::string low_price_time = time.substr(0, time.size() - 6) + "40.000";
		PriceData low_price_data(
			date, low_price_time, low_price, volume
		);
		data_vector.push_back(low_price_data);

		std::string close_price_time = time.substr(0, time.size() - 6) + "59.000";
		PriceData close_price_data(
			date, close_price_time, close_price, volume
		);
		data_vector.push_back(close_price_data);
	}
};