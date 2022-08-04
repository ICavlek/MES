#include <string>

class PriceData {
public:
	PriceData(std::string date, std::string time, double price, double volume) :
		date_time(date + " " + time),
		price(price),
		volume(volume) {}

	std::string date_time;
	double price;
	double volume;
};