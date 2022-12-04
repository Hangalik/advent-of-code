#include <iostream>
#include <utility>

#include "input_selector.h"

std::pair<int, int> getPair(const std::string& str) {
	auto dash = str.find("-");
	std::string first = str.substr(0, dash);
	std::string second = str.substr(dash + 1);
	return { std::stoi(first), std::stoi(second) };
}

bool oneContainsOther(const std::pair<int, int>& first, const std::pair<int, int>& second) {
	return first.first <= second.first && first.second >= second.second;
}

int f1(std::istream& in) {
	std::string firstPair, secondPair;
	int cnt = 0;
	while (std::getline(in, firstPair, ',') && std::getline(in, secondPair)) {
		auto first = getPair(firstPair);
		auto second = getPair(secondPair);
		if (oneContainsOther(first, second) || oneContainsOther(second, first)) {
			cnt++;
		}
	}

	std::cout << cnt << "\n";

	return 0;
}

bool oneOverlapsOther(const std::pair<int, int>& first, const std::pair<int, int>& second) {
	return first.first <= second.first && first.second >= second.first;
}

int f2(std::istream& in) {
	std::string firstPair, secondPair;
	int cnt = 0;
	while (std::getline(in, firstPair, ',') && std::getline(in, secondPair)) {
		auto first = getPair(firstPair);
		auto second = getPair(secondPair);
		if (oneOverlapsOther(first, second) || oneOverlapsOther(second, first)) {
			cnt++;
		}
	}

	std::cout << cnt << "\n";

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