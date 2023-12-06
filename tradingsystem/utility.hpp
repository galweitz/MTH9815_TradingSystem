#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "executionservice.hpp"
#include "pricingservice.hpp"
#include "tradebookingservice.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

template <typename T> std::ostream& operator << (std::ostream& os, std::vector<T>& vec)
{
	for (unsigned int i = 0; i < vec.size(); ++i) {os << vec[i] << " ";}
	return os;
}

std::vector<std::string> tokenize(const std::string& text, const char& delim)
{
	std::vector<std::string> vec;
	std::istringstream input;
	input.str(text);
	for (std::string line; std::getline(input, line, delim); )
	{
		vec.push_back(line);
	}
	return vec;
}

double StringToNum(const std::string& priceString)
{
	std::vector<std::string> vecDigits = tokenize(priceString, '-');
	double integer = std::stoi(vecDigits[0]);
	double digits12 = std::stoi(vecDigits[1].substr(0, 2));
	double digits3 = std::stoi(vecDigits[1].substr(2,3));
	double price = integer + digits12 / 32.0 + digits3 / 256.0;
	return price;
}

#endif