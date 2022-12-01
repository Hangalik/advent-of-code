#include <iostream>
#include <string>
#include <set>
#include <numeric>
#include <ranges>
#include <algorithm>

#include "input_selector.h"

int f1(std::istream& in) {
	int max_sum = 0, current_sum = 0;
	std::string current;
	while (std::getline(in, current)) {
		if (current.empty()) {
			if (current_sum > max_sum) {
				max_sum = current_sum;
			}
			current_sum = 0;
		} else {
			int currenti = std::stoi(current);
			current_sum += currenti;
		}
	}
	if (current_sum > max_sum) {
		max_sum = current_sum;
	}

	std::cout << max_sum << "\n";

	return 0;
}

struct Accumulator {
	void operator()(int val) {
		sum += val;
	}
	int sum = 0;
};

int f2(std::istream& in) {
	int current_sum = 0;
	std::set<int> sums;
	std::string current;
	while (std::getline(in, current)) {
		if (current.empty()) {
			sums.insert(current_sum);
			current_sum = 0;
		} else {
			int currenti = std::stoi(current);
			current_sum += currenti;
		}
	}
	sums.insert(current_sum);

	auto [i, s] = std::ranges::for_each(sums | std::views::reverse | std::views::take(3), Accumulator{});

	std::cout << s.sum << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);

}