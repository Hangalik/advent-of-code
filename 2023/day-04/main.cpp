#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>
#include <ranges>
#include <functional>

#include "input_selector.h"

std::set<int> getNumbers(std::string in) {
	std::set<int> numbers;
	std::stringstream inStream(std::move(in));
	int num;
	while (inStream >> num) {
		numbers.insert(num);
	}
	return numbers;
}

size_t getNumberOfPickedWinningNumbers(std::string line) {
	std::string numbers = line.substr(line.find(':') + 1);
	size_t separator = numbers.find('|');
	std::set<int> winningNumbers = getNumbers(numbers.substr(0, separator));
	std::set<int> pickedNumbers = getNumbers(numbers.substr(separator + 1));
	std::set<int> pickedWinningNumbers;
	std::ranges::set_intersection(winningNumbers, pickedNumbers, std::inserter(pickedWinningNumbers, pickedWinningNumbers.end()));
	return pickedWinningNumbers.size();
}

int f1(std::istream& in) {
	std::string line;
	int sum = 0;
	while (std::getline(in, line)) {
		size_t pickedWinningNumbersNum = getNumberOfPickedWinningNumbers(line);
		if (pickedWinningNumbersNum > 0) {
			sum += 1 << (pickedWinningNumbersNum - 1);
		}
	}

	std::cout << sum << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string line;
	std::unordered_map<int, int> numberOfCards;
	int currentCard = 0;
	while (std::getline(in, line)) {
		currentCard++;
		int& currentCardCount = numberOfCards[currentCard];
		currentCardCount++;
		size_t pickedWinningNumbersNum = getNumberOfPickedWinningNumbers(line);

		for (size_t i = 1; i <= pickedWinningNumbersNum; i++) {
			numberOfCards[currentCard + i] += currentCardCount;
		}
	}

	std::optional<int> allCardCount = std::ranges::fold_left_first(numberOfCards | std::views::values, std::plus<>());
	if (allCardCount.has_value()) {
		std::cout << *allCardCount << "\n";
	} else {
		std::cout << "no cards\n";
	}

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}