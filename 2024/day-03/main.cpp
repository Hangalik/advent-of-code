#include <iostream>
#include <regex>
#include <string>

#include "input_selector.h"

int getSumOfMuls(const std::string& str) {
	int sum = 0;
	std::regex mulRegex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
	std::sregex_iterator mulsBegin(str.begin(), str.end(), mulRegex);
	std::sregex_iterator mulsEnd;
	for (auto i = mulsBegin; i != mulsEnd; ++i) {
		std::smatch match = *i;
		int a = std::stoi(match.str(1));
		int b = std::stoi(match.str(2));
		sum += a * b;
	}
	return sum;
}

int f1(std::istream& in) {
	int sum = 0;
	std::string line;
	while (std::getline(in, line)) {
		sum += getSumOfMuls(line);
	}

	std::cout << sum << "\n";

	return 0;
}

int getSumOfMuls2(const std::string& str, bool& doo) {
	int sum = 0;
	std::regex mulRegex("mul\\((\\d{1,3}),(\\d{1,3})\\)|do\\(\\)|don't\\(\\)");
	std::sregex_iterator mulsBegin(str.begin(), str.end(), mulRegex);
	std::sregex_iterator mulsEnd;
	for (auto i = mulsBegin; i != mulsEnd; ++i) {
		std::smatch match = *i;
		std::string command = match.str(0);
		if (command == "do()") {
			doo = true;
		} else if (command == "don't()") {
			doo = false;
		} else if (doo) {
			int a = std::stoi(match.str(1));
			int b = std::stoi(match.str(2));
			sum += a * b;
		}
	}
	return sum;
}

int f2(std::istream& in) {
	int sum = 0;
	std::string line;
	bool doo = true;
	while (std::getline(in, line)) {
		sum += getSumOfMuls2(line, doo);
	}

	std::cout << sum << "\n";
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}