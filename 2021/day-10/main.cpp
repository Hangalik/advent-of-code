#include <iostream>
#include <vector>
#include <stack>
#include <set>

#include "input_selector.h"

bool isOpeningBrace(char c) {
	return c == '(' || c == '[' || c == '{' || c == '<';
}

bool isClosingBrace(char c) {
	return c == ')' || c == ']' || c == '}' || c == '>';
}

bool isMatching(char opener, char closer) {
	return (opener == '(' && closer == ')') ||
			(opener == '[' && closer == ']') ||
			(opener == '{' && closer == '}') ||
			(opener == '<' && closer == '>');
}

int corruptedScoreFor(char c) {
	if (c == ')') {
		return 3;
	} else if (c == ']') {
		return 57;
	} else if (c == '}') {
		return 1197;
	} else if (c == '>') {
		return 25137;
	}
	throw std::invalid_argument("cannot calculate corrupted score for " + c);
}

int f1(std::istream& in) {
	std::string line;
	int corruptedScore = 0;
	while (in >> line) {
		std::stack<char, std::vector<char>> openers;
		for (size_t i = 0; i < line.size(); i++) {
			char c = line[i];
			if (isOpeningBrace(c)) {
				openers.push(c);
			} else if (isClosingBrace(c)) {
				if (isMatching(openers.top(), c)) {
					openers.pop();
				} else {
					corruptedScore += corruptedScoreFor(c);
					break;
				}
			}
		}
	}

	std::cout << corruptedScore << std::endl;

	return 0;
}

int autocompleteScoreFor(char c) {
	if (c == '(') {
		return 1;
	} else if (c == '[') {
		return 2;
	} else if (c == '{') {
		return 3;
	} else if (c == '<') {
		return 4;
	}
	throw std::invalid_argument("cannot calculate autocomplete score for " + c);
}

int f2(std::istream& in) {
	std::string line;
	std::set<long long> autocompleteScores;
	while (in >> line) {
		std::stack<char, std::vector<char>> openers;
		for (size_t i = 0; i < line.size(); i++) {
			char c = line[i];
			if (isOpeningBrace(c)) {
				openers.push(c);
			} else if (isClosingBrace(c)) {
				if (isMatching(openers.top(), c)) {
					openers.pop();
				} else {
					openers = std::stack<char, std::vector<char>>();
					break;
				}
			}
		}
		if (!openers.empty()) {
			long long autocompleteScore = 0;
			size_t missingCloserNumber = openers.size();
			for (size_t i = 0; i < missingCloserNumber; i++) {
				autocompleteScore *= 5;
				autocompleteScore += autocompleteScoreFor(openers.top());
				openers.pop();
			}
			autocompleteScores.insert(autocompleteScore);
		}
	}
	auto middle = autocompleteScores.begin();
	std::advance(middle, autocompleteScores.size() / 2);
	std::cout << *middle << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	}
}
