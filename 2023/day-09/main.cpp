#include <iostream>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <vector>

#include "input_selector.h"

int predictNextValue(const std::vector<int>& sequence) {
	int prediction = 0;
	std::vector<int> current = sequence;

	while (true) {
		if (std::ranges::all_of(current, [](int i) { return i == 0; })) {
			break;
		}
		prediction += *current.rbegin();
		auto next = current | std::views::pairwise_transform([](int a, int b) { return b - a; });
		current = std::vector<int>(std::ranges::begin(next), std::ranges::end(next));
	}

	return prediction;
}

int f1(std::istream& in) {
	long long sum = 0;
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		std::vector<int> sequence;
		int value;
		while (ss >> value) {
			sequence.push_back(value);
		}
		sum += predictNextValue(sequence);
	}

	std::cout << sum << "\n";
	return 0;
}

int predictPreviousValue(const std::vector<int>& sequence) {
	int prediction = 0;
	bool plus = true;
	std::vector<int> current = sequence;

	while (true) {
		if (std::ranges::all_of(current, [](int i) { return i == 0; })) {
			break;
		}
		if (plus) {
			prediction += *current.begin();
		} else {
			prediction -= *current.begin();
		}
		plus = !plus;
		auto next = current | std::views::pairwise_transform([](int a, int b) { return b - a; });
		current = std::vector<int>(std::ranges::begin(next), std::ranges::end(next));
	}

	return prediction;
}

int f2(std::istream& in) {
	long long sum = 0;
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		std::vector<int> sequence;
		int value;
		while (ss >> value) {
			sequence.push_back(value);
		}
		sum += predictPreviousValue(sequence);
	}

	std::cout << sum << "\n";
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}