#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "input_selector.h"

int f1(std::istream& in) {
	std::vector<std::vector<int>> shortestPaths;
	std::string line;
	while (std::getline(in, line, '\n')) {
		std::cout << line << std::endl;
		std::vector<int> shortestPath;
		for (size_t i = 0; i < line.size(); i++) {
			int riskLevel = line[i] - '0';
			if (i > 0u) {
				if (!shortestPaths.empty()) {
					riskLevel += std::min(shortestPaths.back()[i], shortestPath[i - 1]);
				} else {
					riskLevel += shortestPath[i - 1];
				}
			} else {
				if (!shortestPaths.empty()) {
					riskLevel += shortestPaths.back()[i];
				} else {
					riskLevel = 0;
				}
			}
			std::cout << std::setw(3) << riskLevel;
			shortestPath.push_back(riskLevel);
		}
		std::cout << std::endl;
		shortestPaths.push_back(std::move(shortestPath));
	}

	std::cout << shortestPaths.back().back() << std::endl;

	return 0;
}

int f2(std::istream& ) {

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
}
