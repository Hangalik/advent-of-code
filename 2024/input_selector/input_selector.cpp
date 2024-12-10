#include "input_selector.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <chrono>

static int runTestsWithLogs(std::istream& in, TestFunction& func1, TestFunction& func2, bool measure) {
	std::istreambuf_iterator<char> eos;
	std::istreambuf_iterator<char> inBegin(in);
	std::string inString(inBegin, eos);

	std::istringstream inStream1(inString);
	std::cout << "--- Part 1 ---" << std::endl;
	auto start1 = std::chrono::steady_clock::now();
	if (int val = func1(inStream1); val != 0) {
		return val;
	}
	auto end1 = std::chrono::steady_clock::now();
	if (measure) {
		std::chrono::duration<double> diff = end1 - start1;
		std::cout << "Took " << diff << "\n";
	}

	std::istringstream inStream2(inString);
	std::cout << "\n--- Part 2 ---" << std::endl;
	auto start2 = std::chrono::steady_clock::now();
	if (int val = func2(inStream2); val != 0) {
		return val;
	}
	auto end2 = std::chrono::steady_clock::now();
	if (measure) {
		std::chrono::duration<double> diff = end2 - start2;
		std::cout << "Took " << diff << "\n";
	}

	return 0;
}

static bool isMeasure(int argc, const char* argv[]) {
	if (argc < 3) {
		return false;
	}
	return std::strcmp(argv[2], "--measure") == 0;
}

int runWithProperInput(int argc, const char* argv[], TestFunction func1, TestFunction func2) {
	if (argc != 2 && argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <input-file> [ --measure ]" << std::endl;
		return 1;
	}

	bool measure = isMeasure(argc, argv);
	const char* filename = argv[1];
	if (std::strcmp(filename, "-") == 0) {
		return runTestsWithLogs(std::cin, func1, func2, measure);
	} else {
		if (!std::filesystem::exists(filename)) {
			std::cerr << "The file " << filename << " doesn't exist" << std::endl;
			return 2;
		}
		std::ifstream file(filename);
		return runTestsWithLogs(file, func1, func2, measure);
	}

	return 0;
}