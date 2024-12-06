#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "input_selector.h"

int searchXmas(const std::vector<std::string>& lines, size_t row, size_t column) {
	static constexpr std::array<std::pair<int, int>, 8> searchDirections = {
		std::pair{1, 1},
		std::pair{1, 0},
		std::pair{1, -1},
		std::pair{0, 1},
		std::pair{0, -1},
		std::pair{-1, 1},
		std::pair{-1, 0},
		std::pair{-1, -1}
	};
	static constexpr std::string_view searchPattern = "XMAS";

	int goodDirections = 0;
	for (auto [r, c] : searchDirections) {
		bool equal = true;
		for (size_t pos = 0; pos < searchPattern.length(); pos++) {
			int rowDiff = (int) pos * r;
			int newRow = (int) row + rowDiff;
			if (newRow < 0 || newRow >= (int) lines.size()) {
				equal = false;
				break;
			}
			int colDiff = (int) pos * c;
			int newCol = (int) column + colDiff;
			if (newCol < 0 || newCol >= (int) lines[newRow].size()) {
				equal = false;
				break;
			}
			if (lines[newRow][newCol] != searchPattern[pos]) {
				equal = false;
				break;
			}
		}
		if (equal) {
			goodDirections++;
		}
	}

	return goodDirections;
}

int f1(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(in, line)) {
		lines.push_back(line);
	}

	int xmasCount = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			xmasCount += searchXmas(lines, i, j);
		}
	}

	std::cout << xmasCount << "\n";

	return 0;
}

int searchX_mas(const std::vector<std::string>& lines, size_t row, size_t column) {
	if (lines[row][column] != 'A') {
		return 0;
	}
	if (row < 1 || row >= lines.size() - 1 || column < 1 || column >= lines[row].size() - 1) {
		return 0;
	}
	bool rightMas = lines[row - 1][column - 1] == 'M' && lines[row + 1][column + 1] == 'S';
	bool rightSam = lines[row - 1][column - 1] == 'S' && lines[row + 1][column + 1] == 'M';
	bool leftMas  = lines[row - 1][column + 1] == 'M' && lines[row + 1][column - 1] == 'S';
	bool leftSam  = lines[row - 1][column + 1] == 'S' && lines[row + 1][column - 1] == 'M';

	if ((rightMas || rightSam) && (leftMas || leftSam)) {
		return 1;
	} else {
		return 0;
	}
}

int f2(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(in, line)) {
		lines.push_back(line);
	}

	int x_masCount = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			x_masCount += searchX_mas(lines, i, j);
		}
	}

	std::cout << x_masCount << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}