#include "input_selector.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

static int runTestsWithLogs(std::istream& in, TestFunction& func1, TestFunction& func2) {
	std::istreambuf_iterator<char> eos;
	std::istreambuf_iterator<char> inBegin(in);
	std::string inString(inBegin, eos);

	std::istringstream inStream1(inString);
	std::cout << "--- Part 1 ---" << std::endl;
	if (int val = func1(inStream1); val != 0) {
		return val;
	}

	std::istringstream inStream2(inString);
	std::cout << "\n--- Part 2 ---" << std::endl;
	if (int val = func2(inStream2); val != 0) {
		return val;
	}

	return 0;
}

int runWithProperInput(int argc, const char* argv[], TestFunction func1, TestFunction func2) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
		return 1;
	}

	const char* filename = argv[1];
	if (std::strcmp(filename, "-") == 0) {
		return runTestsWithLogs(std::cin, func1, func2);
	} else {
		if (!std::filesystem::exists(filename)) {
			std::cerr << "The file " << filename << " doesn't exist" << std::endl;
			return 2;
		}
		std::ifstream file(filename);
		return runTestsWithLogs(file, func1, func2);
	}

	return 0;
}