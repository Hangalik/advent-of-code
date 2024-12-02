#include <iostream>
#include <sstream>
#include <vector>

#include "input_selector.h"

std::vector<int> getLevels(const std::string& report) {
	std::vector<int> levels;
	std::stringstream ss(report);
	int level;
	while (ss >> level) {
		levels.push_back(level);
	}
	return levels;
}

bool isSafe(const std::vector<int>& report) {
	bool known = false;
	bool increaseKnown = false;
	bool increasing = false;
	int previous;
	for (int current : report) {
		if (!known) {
			known = true;
			previous = current;
			continue;
		}
		if (!increaseKnown) {
			increaseKnown = true;
			increasing = current > previous;
		}
		if ((increasing && current <= previous) || (!increasing && current >= previous)) {
			return false;
		}
		if (std::abs(current - previous) > 3) {
			return false;
		}
		previous = current;
	}
	return true;
}

int f1(std::istream& in) {
	std::string line;
	int safeCount = 0;
	while (std::getline(in, line)) {
		if (isSafe(getLevels(line))) {
			safeCount++;
		}
	}

	std::cout << safeCount << "\n";

	return 0;
}

bool isSafe2(const std::vector<int>& report) {
	if (isSafe(report)) {
		return true;
	}
	for (unsigned i = 0; i < report.size(); i++) {
		std::vector<int> reportMod = report;
		reportMod.erase(reportMod.begin() + i);
		if (isSafe(reportMod)) {
			return true;
		}
	}
	return false;
}

int f2(std::istream& in) {
	std::string line;
	int safeCount = 0;
	while (std::getline(in, line)) {
		if (isSafe2(getLevels(line))) {
			safeCount++;
		}
	}

	std::cout << safeCount << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}