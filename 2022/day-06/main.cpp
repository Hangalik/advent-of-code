#include <iostream>
#include <unordered_map>

#include "input_selector.h"

int find_unique_segment(std::istream& in, int size) {
	int windowStart = 0;
	int windowEnd = 0;
	std::unordered_map<char, int> lastOccurences;
	for (char c; in.get(c);) {
		if (auto search = lastOccurences.find(c); search != lastOccurences.end()) {
			int lastOcc = search->second;
			bool windowContains = lastOcc > windowStart;
			if (windowContains) {
				windowStart = lastOcc;
			}
		}
		windowEnd++;
		if (windowEnd - windowStart == size) {
			break;
		}
		lastOccurences.insert_or_assign(c, windowEnd);
	}
	return windowEnd;
}

int f1(std::istream& in) {
	int n = find_unique_segment(in, 4);

	std::cout << n << "\n";

	return 0;
}

int f2(std::istream& in) {
	int n = find_unique_segment(in, 14);

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