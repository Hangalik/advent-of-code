#include <iostream>
#include <ranges>
#include <algorithm>
#include <charconv>
#include <vector>

#include "input_selector.h"

std::vector<int> getGroups(const std::string& groups) {
	auto groupsNum = groups | std::views::split(',') | std::views::transform([](auto group) {
		std::string_view groupStr(group);
		int value = 0;
		std::from_chars(groupStr.begin(), groupStr.end(), value);
		return value;
	});
	return std::vector<int>(std::ranges::begin(groupsNum), std::ranges::end(groupsNum));
}

int f1(std::istream& in) {
	std::string line;
	while (std::getline(in, line)) {
		size_t seperation = line.find(' ');
		std::string springs = line.substr(0, seperation);
		std::vector<int> groups = getGroups(line.substr(seperation + 1));
		
	}
	return 0;
}

int f2(std::istream& in) {
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}