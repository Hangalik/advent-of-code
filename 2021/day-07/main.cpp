#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>

#include "input_selector.h"

int lowestFuelCostPosition(const std::map<int, int>& crabs) {
	int crabCount = 0;
	for (const auto& [position, crabsInPosition] : crabs) {
		crabCount += crabsInPosition;
	}
	int belowCrabs = 0;
	for (const auto& [position, crabsInPosition] : crabs) {
		belowCrabs += crabsInPosition;
		int aboveCrabs = crabCount - belowCrabs;
		if (aboveCrabs <= belowCrabs) {
			return position;
		}
	}
	throw std::logic_error("Should have a position at this point");
}

int f1(std::istream& in) {
	std::map<int, int> crabs;
	int num;
	while (in >> num) {
		in.ignore();
		crabs[num]++;
	}

	int cheapestPosition = lowestFuelCostPosition(crabs);

	int fuelCost = 0;
	for (const auto& [position, crabsInPosition] : crabs) {
		fuelCost += std::abs(cheapestPosition - position) * crabsInPosition;
	}

	std::cout << fuelCost << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::map<int, int> crabs;
	int num;
	while (in >> num) {
		in.ignore();
		crabs[num]++;
	}

	const int highestPosition = crabs.rbegin()->first;
	std::vector<int> fuelCosts(highestPosition + 1);
	for (const auto& [position, crabsInPosition] : crabs) {
		for (size_t i = 0; i < fuelCosts.size(); i++) {
			int actPosition = i;
			int distance = std::abs(position - actPosition);
			fuelCosts[i] += (crabsInPosition * distance * (distance + 1)) / 2;
		}
	}

	int fuelCost = *std::min_element(fuelCosts.begin(), fuelCosts.end());
	std::cout << fuelCost << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);
}