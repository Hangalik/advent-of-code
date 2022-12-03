#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

#include "input_selector.h"

int priorityOfItem(char c) {
	if (std::islower(c)) {
		return c - 'a' + 1;
	} else if (std::isupper(c)) {
		return c - 'A' + 27;
	}
	return 0;
}

int f1(std::istream& in) {
	std::string line;
	int sumOfPriority = 0;
	while (std::getline(in, line)) {
		size_t half_length = line.length() / 2;
		std::set<char> firstCompartment(line.begin(), line.begin() + half_length);
		std::set<char> secondCompartment(line.begin() + half_length, line.end());
		std::vector<char> commonItem;
		std::ranges::set_intersection(firstCompartment, secondCompartment, std::back_inserter(commonItem));
		sumOfPriority += priorityOfItem(commonItem.front());
	}

	std::cout << sumOfPriority << '\n';

	return 0;
}

int f2(std::istream& in) {
	std::string line1, line2, line3;
	int sumOfPriority = 0;
	while (std::getline(in, line1) && std::getline(in, line2) && std::getline(in, line3)) {
		std::set<char> bag1(line1.begin(), line1.end());
		std::set<char> bag2(line2.begin(), line2.end());
		std::set<char> bag3(line3.begin(), line3.end());

		std::vector<char> bag12;
		std::ranges::set_intersection(bag1, bag2, std::back_inserter(bag12));
		std::vector<char> bag123;
		std::ranges::set_intersection(bag12, bag3, std::back_inserter(bag123));
		sumOfPriority += priorityOfItem(bag123.front());
	}

	std::cout << sumOfPriority << '\n';

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