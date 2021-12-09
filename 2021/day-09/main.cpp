#include <iostream>
#include <string>
#include <vector>

#include "input_selector.h"

int f1(std::istream& in) {
	std::vector<std::vector<int>> floorHeight;
	std::string line;
	while (in >> line) {
		std::vector<int> lineHeight(line.size());
		for (size_t i = 0; i < line.size(); i++) {
			int point = std::stoi(line.substr(i, 1));
			lineHeight[i] = point;
		}
		floorHeight.push_back(std::move(lineHeight));
	}
	int riskLevel = 0;
	for (size_t i = 0; i < floorHeight.size(); i++) {
		for (size_t j = 0; j < floorHeight[i].size(); j++) {
			int point = floorHeight[i][j];
			bool isLowPoint = true;
			if (i > 0u) {
				isLowPoint &= floorHeight[i - 1][j] > point;
			}
			if (j > 0u) {
				isLowPoint &= floorHeight[i][j - 1] > point;
			}
			if (i < floorHeight.size() - 1) {
				isLowPoint &= floorHeight[i + 1][j] > point;
			}
			if (j < floorHeight[i].size() - 1) {
				isLowPoint &= floorHeight[i][j + 1] > point;
			}
			if (isLowPoint) {
				riskLevel += 1 + point;
			}
		}
	}

	std::cout << riskLevel << std::endl;

	return 0;
}

int f2(std::istream& ) {

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);
}
