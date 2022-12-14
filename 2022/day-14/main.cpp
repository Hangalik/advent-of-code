#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <unordered_set>

#include "input_selector.h"

struct PairHasher {
	size_t operator()(const std::pair<int, int>& v) const {
		return 31 * std::hash<int>{}(v.first) + std::hash<int>{}(v.second);
	}
};

std::pair<int, int> readInPair(const std::string& in) {
	size_t dividerPos = in.find(',');
	int first = std::stoi(in.substr(0, dividerPos));
	int second = std::stoi(in.substr(dividerPos + 1));
	return {first, second};
}

std::vector<std::pair<int, int>> readInPairs(std::string in) {
	std::vector<std::pair<int, int>> result;

	size_t i = 0;
	while (true) {
		size_t dividerPos = in.find(" -> ");
		if (dividerPos != std::string::npos) {
			std::string pairStr = in.substr(0, dividerPos);
			result.push_back(readInPair(pairStr));
			in = in.substr(dividerPos + 4);
		} else {
			result.push_back(readInPair(in));
			break;
		}
	}

	return result;
}

int signum(int a) {
	if (a < 0) {
		return -1;
	}
	if (a > 0) {
		return 1;
	}
	return 0;
}

std::pair<int, int> stepOne(std::pair<int, int> first, std::pair<int, int> second) {
	std::pair<int, int> diff = {second.first - first.first, second.second - first.second};
	first.first += signum(diff.first);
	first.second += signum(diff.second);
	return first;
}

void addAllInBetween(std::unordered_set<std::pair<int, int>, PairHasher>& occupiance,
					std::pair<int, int> first, std::pair<int, int> second)
{
	occupiance.insert(first);
	while (first != second) {
		first = stepOne(first, second);
		occupiance.insert(first);
	}
}

std::unordered_set<std::pair<int, int>, PairHasher> readInRocks(std::istream& in) {
	std::string line;
	std::unordered_set<std::pair<int, int>, PairHasher> occupiance;
	while (std::getline(in, line)) {
		auto coords = readInPairs(line);
		for (auto first = coords.begin(), second = coords.begin() + 1; second != coords.end(); first++, second++) {
			addAllInBetween(occupiance, *first, *second);
		}
	}
	return occupiance;
}

int findLowestRock(const std::unordered_set<std::pair<int, int>, PairHasher>& rocks) {
	int lowestRock = -1;
	for (const auto& rock : rocks) {
		lowestRock = std::max(lowestRock, rock.second);
	}
	return lowestRock;
}

std::tuple<std::pair<int, int>, std::pair<int, int>, std::pair<int, int>> orderOfCheck(const std::pair<int, int>& coord) {
	std::pair<int, int> first = {coord.first, coord.second + 1};
	std::pair<int, int> second = {coord.first - 1, coord.second + 1};
	std::pair<int, int> third = {coord.first + 1, coord.second + 1};
	return {first, second, third};
}

static constexpr std::pair<int, int> sandSource{500, 0};

int f1(std::istream& in) {
	auto occupiance = readInRocks(in);
	int lowestRock = findLowestRock(occupiance);
	int sandCount = 0;
	while (true) {
		std::pair<int, int> sand = sandSource;
		bool fallToAbyss = false;
		while (true) {
			if (sand.second >= lowestRock) {
				fallToAbyss = true;
				break;
			}
			auto [first, second, third] = orderOfCheck(sand);
			if (!occupiance.contains(first)) {
				sand = first;
			} else if (!occupiance.contains(second)) {
				sand = second;
			} else if (!occupiance.contains(third)) {
				sand = third;
			} else {
				break;
			}
		}
		if (fallToAbyss) {
			break;
		}
		sandCount++;
		occupiance.insert(sand);
	}

	std::cout << sandCount << "\n";

	return 0;
}

int f2(std::istream& in) {
	auto occupiance = readInRocks(in);
	int floor = findLowestRock(occupiance) + 2;
	int sandCount = 0;
	while (true) {
		std::pair<int, int> sand = sandSource;
		while (true) {
			if (sand.second == floor - 1) {
				break;
			}
			auto [first, second, third] = orderOfCheck(sand);
			if (!occupiance.contains(first)) {
				sand = first;
			} else if (!occupiance.contains(second)) {
				sand = second;
			} else if (!occupiance.contains(third)) {
				sand = third;
			} else {
				break;
			}
		}
		sandCount++;
		occupiance.insert(sand);
		if (sand == sandSource) {
			break;
		}
	}

	std::cout << sandCount << "\n";

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