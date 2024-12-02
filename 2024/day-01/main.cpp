#include <iostream>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <unordered_map>

#include "input_selector.h"

int f1(std::istream& in) {
	std::string line;
	std::vector<int> as, bs;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		int a, b;
		ss >> a >> b;
		as.push_back(a);
		bs.push_back(b);
	}
	std::ranges::sort(as);
	std::ranges::sort(bs);

	int diffs = 0;
	for (auto [a, b] : std::views::zip(as, bs)) {
		diffs += std::abs(a - b);
	}

	std::cout << diffs << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string line;
	std::vector<int> as;
	std::unordered_map<int, int> bs;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		int a, b;
		ss >> a >> b;
		as.push_back(a);
		bs[b]++;
	}

	int simmilarity = 0;
	for (int a : as) {
		if (bs.contains(a)) {
			simmilarity += a * bs[a];
		}
	}

	std::cout << simmilarity << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}