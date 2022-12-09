#include <iostream>
#include <unordered_set>
#include <array>
#include <algorithm>

#include "input_selector.h"

struct PairHasher {
	size_t operator()(const std::pair<int, int>& v) const {
		return 31 * std::hash<int>{}(v.first) + std::hash<int>{}(v.second);
	}
};

int signum(int v) {
	if (v < 0) {
		return -1;
	}
	if (v > 0) {
		return 1;
	}
	return 0;
}

std::pair<int, int> offset(const std::pair<int, int>& p) {
	if (std::abs(p.first) <= 1 && std::abs(p.second) <= 1) {
		return {0, 0};
	}
	return {signum(p.first), signum(p.second)};
}

std::pair<int, int> updateTail(const std::pair<int, int>& head, const std::pair<int, int>& tail) {
	std::pair<int, int> reference = {head.first - tail.first, head.second - tail.second};
	std::pair<int, int> offs = offset(reference);
	return {tail.first + offs.first, tail.second + offs.second};
}

void printPair(const std::pair<int, int>& p, const std::string& name) {
	std::cout << name <<"{" << p.first << "," << p.second << "}\n";
}

int f1(std::istream& in) {
	std::string line;

	std::pair<int, int> head(0, 0);
	std::pair<int, int> tail(0, 0);
	std::unordered_set<std::pair<int, int>, PairHasher> tailPositions;
	tailPositions.insert(tail);
	while (std::getline(in, line)) {
		int stepNum = std::stoi(line.substr(2));
		char direction = line[0];
		for (int i = 0; i < stepNum; i++) {
			if (direction == 'R') {
				head.first++;
			} else if (direction == 'L') {
				head.first--;
			} else if (direction == 'U') {
				head.second++;
			} else if (direction == 'D') {
				head.second--;
			}
			tail = updateTail(head, tail);
			tailPositions.insert(tail);
		}
	}

	std::cout << tailPositions.size() << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string line;

	std::array<std::pair<int, int>, 10> knots;
	std::fill(knots.begin(), knots.end(), std::pair<int, int>{0, 0});
	std::unordered_set<std::pair<int, int>, PairHasher> tailPositions;
	tailPositions.insert(knots.back());
	while (std::getline(in, line)) {
		int stepNum = std::stoi(line.substr(2));
		char direction = line[0];
		for (int i = 0; i < stepNum; i++) {
			if (direction == 'R') {
				knots[0].first++;
			} else if (direction == 'L') {
				knots[0].first--;
			} else if (direction == 'U') {
				knots[0].second++;
			} else if (direction == 'D') {
				knots[0].second--;
			}
			for (size_t i = 1; i < knots.size(); i++) {
				knots[i] = updateTail(knots[i - 1], knots[i]);
			}
			tailPositions.insert(knots.back());
		}
	}

	std::cout << tailPositions.size() << "\n";

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