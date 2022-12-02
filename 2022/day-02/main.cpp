#include <iostream>
#include <unordered_map>

#include "input_selector.h"

int pointsForMatch(char you, char opp) {
	int points = you - 'X' + 1;
	if ((you == 'X' && opp == 'C') || (you == 'Y' && opp == 'A') || (you == 'Z' && opp == 'B')) {
		points += 6;
	} else if (you == (opp - 'A' + 'X')) {
		points += 3;
	}
	return points;
}

int f1(std::istream& in) {
	std::string line;
	int points = 0;
	while (std::getline(in, line)) {
		char opp = line[0];
		char you = line[2];
		points += pointsForMatch(you, opp);
	}

	std::cout << points << '\n';

	return 0;
}

enum class Hand {
	Rock, Paper, Scissors
};

Hand fromOpp(char opp) {
	return static_cast<Hand>(opp - 'A');
}

Hand fromYou(char you) {
	return static_cast<Hand>(you - 'X');
}

static const std::unordered_map<Hand, Hand> winsOver{
	{Hand::Rock, Hand::Scissors},
	{Hand::Paper, Hand::Rock},
	{Hand::Scissors, Hand::Paper}
};

static const std::unordered_map<Hand, Hand> losesTo{
	{Hand::Rock, Hand::Paper},
	{Hand::Paper, Hand::Scissors},
	{Hand::Scissors, Hand::Rock}
};

int pointsForMatch(Hand you, Hand opp) {
	int points = static_cast<int>(you) + 1;
	if (winsOver.at(you) == opp) {
		points += 6;
	} else if (losesTo.at(you) != opp) {
		points += 3;
	}
	return points;
}

Hand youFromOppAndOutcome(Hand opp, char outcome) {
	if (outcome == 'X') {
		return winsOver.at(opp);
	} else if (outcome == 'Y') {
		return opp;
	} else {
		return losesTo.at(opp);
	}
}

int f2(std::istream& in) {
	std::string line;
	int points = 0;
	while (std::getline(in, line)) {
		Hand opp = fromOpp(line[0]);
		char outcome = line[2];
		Hand you = youFromOppAndOutcome(opp, outcome);
		points += pointsForMatch(you, opp);
	}

	std::cout << points << '\n';

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