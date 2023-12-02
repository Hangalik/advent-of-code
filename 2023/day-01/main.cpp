#include <string>
#include <iostream>
#include <utility>

#include "input_selector.h"

int f1(std::istream& in) {
	std::string line;
	int sum = 0;
	while (std::getline(in, line)) {
		int firstDigit = -1;
		int lastDigit = -1;
		for (char c : line) {
			if (std::isdigit(c)) {
				lastDigit = c - '0';
				if (firstDigit == -1) {
					firstDigit = c - '0';
				}
			}
		}
		sum += firstDigit * 10 + lastDigit;
	}
	std::cout << sum << "\n";
	return 0;
}

std::pair<std::string, int> nameValues[] = {
	{"zero", 0},
	{"one", 1},
	{"two", 2},
	{"three", 3},
	{"four", 4},
	{"five", 5},
	{"six", 6},
	{"seven", 7},
	{"eight", 8},
	{"nine", 9}
};

int f2(std::istream& in) {
	std::string line;
	int sum = 0;
	int i = 1;
	while (std::getline(in, line)) {
		size_t firstDigitPos = std::string::npos;
		size_t lastDigitPos = std::string::npos;
		int firstDigit = -1;
		int lastDigit = -1;

		for (auto [name, value] : nameValues) {
			size_t namePos = line.find(name);
			if (namePos == std::string::npos) {
				continue;
			}
			if (firstDigitPos == std::string::npos || namePos < firstDigitPos) {
				firstDigitPos = namePos;
				firstDigit = value;
			}
			namePos = line.rfind(name);
			if (lastDigitPos == std::string::npos || namePos > lastDigitPos) {
				lastDigitPos = namePos;
				lastDigit = value;
			}
		}

		for (size_t i = 0; i < line.size(); i++) {
			if (std::isdigit(line[i])) {
				if (firstDigitPos == std::string::npos || i < firstDigitPos) {
					firstDigitPos = i;
					firstDigit = line[i] - '0';
				}
				if (lastDigitPos == std::string::npos || i > lastDigitPos) {
					lastDigitPos = i;
					lastDigit = line[i] - '0';
				}
			}
		}

		sum += firstDigit * 10 + lastDigit;
	}
	std::cout << sum << "\n";
	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);
}