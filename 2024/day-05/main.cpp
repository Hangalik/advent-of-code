#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "input_selector.h"

std::vector<int> getPages(const std::string& line) {
	std::vector<int> pages;
	std::string token;
	std::istringstream iss(line);
	while (std::getline(iss, token, ',')) {
		pages.push_back(std::stoi(token));
	}
	return pages;
}

std::unordered_multimap<int, int> parseRules(std::istream& in) {
	std::unordered_multimap<int, int> orderRule;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) {
			break;
		}
		int left = std::stoi(line.substr(0, 2));
		int right = std::stoi(line.substr(3, 2));
		orderRule.emplace(left, right);
	}
	return orderRule;
}

struct Comparer {
	explicit Comparer(std::unordered_multimap<int, int> rules) : rules(std::move(rules)) {}

	bool operator()(int a, int b) const {
		auto range = rules.equal_range(a);
		auto found = std::ranges::find(range.first, range.second, b, &std::unordered_multimap<int, int>::value_type::second);
		return found != range.second;
	}

private:
	std::unordered_multimap<int, int> rules;
};

int f1(std::istream& in) {

	Comparer comp(parseRules(in));

	std::string line;
	int sum = 0;
	while (std::getline(in, line)) {
		std::vector<int> pages = getPages(line);
		bool sorted = std::ranges::is_sorted(pages, comp);
		if (sorted) {
			sum += pages[pages.size() / 2];
		}
	}

	std::cout << sum << "\n";

	return 0;
}

int f2(std::istream& in) {

	Comparer comp(parseRules(in));

	std::string line;
	int sum = 0;
	while (std::getline(in, line)) {
		std::vector<int> pages = getPages(line);
		bool sorted = std::ranges::is_sorted(pages, comp);
		if (!sorted) {
			auto middle = pages.begin() + (pages.size() / 2);
			std::ranges::nth_element(pages, middle, comp);
			sum += *middle;
		}
	}

	std::cout << sum << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}