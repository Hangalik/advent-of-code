#include <iostream>
#include <list>
#include <unordered_set>

#include "input_selector.h"

bool is_all_unique(const std::list<char>& chars) {
	std::unordered_set<char> uniques;
	for (char c : chars) {
		if (uniques.contains(c)) {
			return false;
		}
		uniques.insert(c);
	}
	return true;
}

int f1(std::istream& in) {
	int n = 0;
	std::list<char> characters;
	for (char c; in.get(c);) {
		characters.push_back(c);
		n++;
		if (characters.size() > 4) {
			characters.pop_front();
		}
		if (characters.size() == 4) {
			if (is_all_unique(characters)) {
				break;
			}
		}
	}

	std::cout << n << "\n";

	return 0;
}

int f2(std::istream& in) {
	int n = 0;
	std::list<char> characters;
	for (char c; in.get(c);) {
		characters.push_back(c);
		n++;
		if (characters.size() > 14) {
			characters.pop_front();
		}
		if (characters.size() == 14) {
			if (is_all_unique(characters)) {
				break;
			}
		}
	}

	std::cout << n << "\n";

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