#include <string>

class PriceData {
public:
	PriceData(std::string date, std::string time, double price, double volume) :
		date(date),
		time(time),
		price(price),
		volume(volume) {}

	std::string date;
	std::string time;
	double price;
	double volume;
};