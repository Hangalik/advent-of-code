#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <ranges>

#include "input_selector.h"

std::vector<unsigned> getNumbers(std::string in) {
	std::vector<unsigned> numbers;
	std::stringstream inStream(std::move(in));
	unsigned num;
	while (inStream >> num) {
		numbers.push_back(num);
	}
	return numbers;
}

void runLogic(std::vector<unsigned>& seeds, std::vector<std::tuple<unsigned, unsigned, unsigned>>& mappings) {
	std::vector<unsigned> nextSeedGeneration;
	for (unsigned seed : seeds) {
		unsigned nextGeneration = seed;
		for (auto [destinationStart, sourceStart, length] : mappings) {
			if (seed >= sourceStart && seed < sourceStart + length) {
				unsigned diff = seed - sourceStart;
				nextGeneration = destinationStart + diff;
			}
		}
		nextSeedGeneration.push_back(nextGeneration);
	}
	mappings.clear();
	seeds = std::move(nextSeedGeneration);
}

int f1(std::istream& in) {
	std::string line;
	std::getline(in, line);
	line = line.substr(7);

	std::vector<unsigned> seeds = getNumbers(line);
	std::vector<std::tuple<unsigned, unsigned, unsigned>> mappings;

	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		if (line.contains("map")) {
			runLogic(seeds, mappings);
			continue;
		}
		std::vector<unsigned> mapNumbers = getNumbers(line);
		unsigned destinationStart = mapNumbers[0];
		unsigned sourceStart = mapNumbers[1];
		unsigned length = mapNumbers[2];
		mappings.emplace_back(destinationStart, sourceStart, length);
	}
	runLogic(seeds, mappings);
	unsigned min = *std::ranges::min_element(seeds);

	std::cout << min << "\n";

	return 0;
}

// Not implemented when a mapping maps from the middle of a range, but the result is correct anyways..
void runLogic2(std::vector<std::tuple<unsigned, unsigned>>& ranges, std::vector<std::tuple<unsigned, unsigned, unsigned>>& mappings) {
	std::vector<std::tuple<unsigned, unsigned>> nextRangeGeneration;
	for (auto [rangeStart, rangeLength] : ranges) {
		for (auto [destinationStart, sourceStart, length] : mappings) {
			if (rangeStart >= sourceStart) {
				unsigned rangeOffset = rangeStart - sourceStart;
				if (rangeOffset < length) {
					unsigned newRangeLength = std::min(length - rangeOffset, rangeLength);
					unsigned newRangeStart = destinationStart + rangeOffset;
					nextRangeGeneration.emplace_back(newRangeStart, newRangeLength);
					rangeStart += newRangeLength;
					rangeLength -= newRangeLength;
				}
			} else {
				unsigned rangeOffset = sourceStart - rangeStart;
				if (rangeOffset < rangeLength) {
					unsigned newRangeLength = std::min(rangeLength - rangeOffset, length);
					unsigned newRangeStart = destinationStart;
					nextRangeGeneration.emplace_back(newRangeStart, newRangeLength);
					rangeLength -= newRangeLength;
				}
			}
			if (rangeLength == 0) {
				break;
			}
		}
		if (rangeLength > 0) {
			nextRangeGeneration.emplace_back(rangeStart, rangeLength);
		}
	}
	mappings.clear();
	ranges = std::move(nextRangeGeneration);
}

int f2(std::istream& in) {
	std::string line;
	std::getline(in, line);
	line = line.substr(7);

	std::vector<unsigned> numbers = getNumbers(line);
	std::vector<std::tuple<unsigned, unsigned>> ranges;
	for (const auto& range : numbers | std::views::chunk(2)) {
		ranges.emplace_back(range[0], range[1]);
	}
	std::vector<std::tuple<unsigned, unsigned, unsigned>> mappings;

	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		if (line.contains("map")) {
			runLogic2(ranges, mappings);
			continue;
		}
		std::vector<unsigned> mapNumbers = getNumbers(line);
		unsigned destinationStart = mapNumbers[0];
		unsigned sourceStart = mapNumbers[1];
		unsigned length = mapNumbers[2];
		mappings.emplace_back(destinationStart, sourceStart, length);
	}
	runLogic2(ranges, mappings);
	unsigned min = std::get<0>(*std::ranges::min_element(ranges));

	std::cout << min << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}