#include <iostream>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <vector>
#include <set>
#include <algorithm>
#include <optional>

#include "input_selector.h"

int f1(std::istream& in) {
	// int interestingY = 10;
	int interestingY = 2'000'000;
	std::unordered_set<int> excludedFields;
	std::unordered_set<int> beaconsInInterestingRow;
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream sline(line);
		sline.ignore(12);
		int sx;
		sline >> sx;
		sline.ignore(4);
		int sy;
		sline >> sy;
		sline.ignore(25);
		int bx;
		sline >> bx;
		sline.ignore(4);
		int by;
		sline >> by;
		if (by == interestingY) {
			beaconsInInterestingRow.insert(bx);
		}
		int manhattanDistance = std::abs(sx - bx) + std::abs(sy - by);
		int interestingRowRange = manhattanDistance - std::abs(sy - interestingY);
		for (int i = 0; i <= interestingRowRange; i++) {
			excludedFields.insert(sx + i);
			excludedFields.insert(sx - i);
		}
	}

	int counter = 0;
	for (int field : excludedFields) {
		if (!beaconsInInterestingRow.contains(field)) {
			counter++;
		}
	}

	std::cout << counter << "\n";

	return 0;
}

class Sensor {
public:
	Sensor(int x, int y, int distance) : x(x), y(y), distance(distance) {}

	std::optional<std::pair<int, int>> getRowCoverage(int row) const {
		int rowRange = distance - std::abs(y - row);
		if (rowRange < 0) {
			return {};
		}
		return std::pair<int, int>{x - rowRange, x + rowRange};
	}

private:
	int x, y;
	int distance;
};

int f2(std::istream& in) {
	// int threshold = 20;
	int threshold = 4'000'000;
	std::string line;
	std::vector<Sensor> sensors;
	while (std::getline(in, line)) {
		std::stringstream sline(line);
		sline.ignore(12);
		int sx;
		sline >> sx;
		sline.ignore(4);
		int sy;
		sline >> sy;
		sline.ignore(25);
		int bx;
		sline >> bx;
		sline.ignore(4);
		int by;
		sline >> by;
		int manhattanDistance = std::abs(sx - bx) + std::abs(sy - by);
		sensors.emplace_back(sx, sy, manhattanDistance);
	}

	long long tuningFreq = -1;
	for (int i = 0; i <= threshold; i++) {
		std::set<std::pair<int, int>> excludedRanges;
		for (const Sensor& sensor : sensors) {
			auto rangeOpt = sensor.getRowCoverage(i);
			if (rangeOpt.has_value()) {
				excludedRanges.insert(*rangeOpt);
			}
		}
		int maxSecond = -1;
		for (const std::pair<int, int>& range : excludedRanges) {
			if (range.first - 1 > maxSecond) {
				break;
			}
			maxSecond = std::max(maxSecond, range.second);
		}
		if (maxSecond < threshold) {
			tuningFreq = (maxSecond + 1) * 4'000'000LL + i;
			break;
		}
	}

	std::cout << tuningFreq << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
	return 1;
}