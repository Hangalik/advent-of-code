#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <vector>

#include "input_selector.h"

struct Neighbours {
	std::string left;
	std::string right;
};

int f1(std::istream& in) {
	std::string stepSequence;
	std::getline(in, stepSequence);

	std::unordered_map<std::string, Neighbours> network;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}

		std::string from = line.substr(0, 3);
		std::string left = line.substr(7, 3);
		std::string right = line.substr(12, 3);

		network.emplace(from, Neighbours{left, right});
	}

	std::string current = "AAA";
	int steps = 0;
	while (current != "ZZZ") {
		int dirIndex = steps % stepSequence.size();
		char directionChar = stepSequence[dirIndex];
		if (directionChar == 'L') {
			current = network[current].left;
		} else {
			current = network[current].right;
		}
		steps++;
	}

	std::cout << steps << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string stepSequence;
	std::getline(in, stepSequence);

	std::unordered_map<std::string, Neighbours> network;
	std::vector<std::string> startingPositions;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}

		std::string from = line.substr(0, 3);
		std::string left = line.substr(7, 3);
		std::string right = line.substr(12, 3);

		network.emplace(from, Neighbours{left, right});

		if (from[2] == 'A') {
			startingPositions.push_back(from);
		}
	}


	int steps = 0;
	while (std::ranges::any_of(startingPositions, [](const std::string& a) { return a[2] != 'Z'; })) {
		int dirIndex = steps % stepSequence.size();
		char directionChar = stepSequence[dirIndex];
		for (std::string& current : startingPositions) {
			if (directionChar == 'L') {
				current = network[current].left;
			} else {
				current = network[current].right;
			}
		}
		steps++;
	}

	std::cout << steps << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}