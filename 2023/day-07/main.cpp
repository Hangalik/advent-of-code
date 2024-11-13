#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <vector>

#include "input_selector.h"

enum class CamelHandType {
	HIGHCARD,
	ONEPAIR,
	TWOPAIR,
	THREEOFKIND,
	FULLHOUSE,
	FOUROFKIND,
	FIVEOFKIND
};

static const std::unordered_map<char, int> cardValues({
	{'2', 0},
	{'3', 1},
	{'4', 2},
	{'5', 3},
	{'6', 4},
	{'7', 5},
	{'8', 6},
	{'9', 7},
	{'T', 8},
	{'J', 9},
	{'Q', 10},
	{'K', 11},
	{'A', 12}
});

struct CamelCompare {
	static bool operator()(char a, char b) {
		return cardValues.at(a) < cardValues.at(b);
	}
};

class CamelHand {
public:
	explicit CamelHand(std::string hand) :
		type(CamelHandType::HIGHCARD),
		hand(hand)
	{
		std::unordered_map<char, int> cardNumbersMap;
		for (char card : hand) {
			cardNumbersMap[card]++;
		}
		auto cardNumbers = std::views::values(cardNumbersMap);
		std::vector<int> numbers(std::ranges::begin(cardNumbers), std::ranges::end(cardNumbers));
		std::ranges::sort(numbers, std::greater{});
		if (numbers[0] == 5) {
			type = CamelHandType::FIVEOFKIND;
		} else if (numbers[0] == 4) {
			type = CamelHandType::FOUROFKIND;
		} else if (numbers[0] == 3) {
			if (numbers[1] == 2) {
				type = CamelHandType::FULLHOUSE;
			} else {
				type = CamelHandType::THREEOFKIND;
			}
		} else if (numbers[0] == 2) {
			if (numbers[1] == 2) {
				type = CamelHandType::TWOPAIR;
			} else {
				type = CamelHandType::ONEPAIR;
			}
		} else {
			type = CamelHandType::HIGHCARD;
		}
	}

	friend bool operator<(const CamelHand& a, const CamelHand& b) {
		if (a.type != b.type) {
			return std::to_underlying(a.type) < std::to_underlying(b.type);
		}
		return std::ranges::lexicographical_compare(a.hand, b.hand, CamelCompare{});
	}

private:
	CamelHandType type;
	std::string hand;
};

int f1(std::istream& in) {
	std::map<CamelHand, int> playerHands;
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		std::string hand;
		int bid;
		ss >> hand >> bid;
		playerHands.try_emplace(CamelHand{hand}, bid);
	}

	int totalWinnings = 0;
	for (auto [rank, bid] : playerHands | std::views::values | std::views::enumerate) {
		rank++;
		totalWinnings += rank * bid;
	}

	std::cout << totalWinnings << "\n";

	return 0;
}

static const std::unordered_map<char, int> cardValues2({
	{'J', 0},
	{'2', 1},
	{'3', 2},
	{'4', 3},
	{'5', 4},
	{'6', 5},
	{'7', 6},
	{'8', 7},
	{'9', 8},
	{'T', 9},
	{'Q', 10},
	{'K', 11},
	{'A', 12}
});

struct CamelCompare2 {
	static bool operator()(char a, char b) {
		return cardValues2.at(a) < cardValues2.at(b);
	}
};

class CamelHand2 {
public:
	explicit CamelHand2(std::string hand) :
		type(CamelHandType::HIGHCARD),
		hand(hand)
	{
		std::unordered_map<char, int> cardNumbersMap;
		int jCount = 0;
		for (char card : hand) {
			if (card == 'J') {
				jCount++;
			} else {
				cardNumbersMap[card]++;
			}
		}
		auto cardNumbers = std::views::values(cardNumbersMap);
		std::vector<int> numbers(std::ranges::begin(cardNumbers), std::ranges::end(cardNumbers));
		std::ranges::sort(numbers, std::greater{});
		if (numbers.size() > 0) {
			numbers[0] += jCount;
		} else {
			numbers.push_back(jCount);
		}
		if (numbers[0] == 5) {
			type = CamelHandType::FIVEOFKIND;
		} else if (numbers[0] == 4) {
			type = CamelHandType::FOUROFKIND;
		} else if (numbers[0] == 3) {
			if (numbers[1] == 2) {
				type = CamelHandType::FULLHOUSE;
			} else {
				type = CamelHandType::THREEOFKIND;
			}
		} else if (numbers[0] == 2) {
			if (numbers[1] == 2) {
				type = CamelHandType::TWOPAIR;
			} else {
				type = CamelHandType::ONEPAIR;
			}
		} else {
			type = CamelHandType::HIGHCARD;
		}
	}

	friend bool operator<(const CamelHand2& a, const CamelHand2& b) {
		if (a.type != b.type) {
			return std::to_underlying(a.type) < std::to_underlying(b.type);
		}
		return std::ranges::lexicographical_compare(a.hand, b.hand, CamelCompare2{});
	}

private:
	CamelHandType type;
	std::string hand;
};

int f2(std::istream& in) {
	std::map<CamelHand2, int> playerHands;
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		std::string hand;
		int bid;
		ss >> hand >> bid;
		playerHands.try_emplace(CamelHand2{hand}, bid);
	}

	int totalWinnings = 0;
	for (auto [rank, bid] : playerHands | std::views::values | std::views::enumerate) {
		rank++;
		totalWinnings += rank * bid;
	}

	std::cout << totalWinnings << "\n";
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}