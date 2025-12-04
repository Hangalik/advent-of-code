#include <iostream>
#include <vector>

#include "input_selector.h"

std::pair<int, int> getDiffForChar(char c) {
	switch (c) {
	case '^': return {-1, 0};
	case '>': return {0, 1};
	case 'v': return {1, 0};
	case '<': return {0, -1};
	}
	return {0, 0};
}

std::pair<size_t, size_t> attemptStep(std::vector<std::string>& map, size_t playerRow, size_t playerColumn, char c) {
	auto [diffRow, diffColumn] = getDiffForChar(c);
	size_t row = playerRow + diffRow;
	size_t column = playerColumn + diffColumn;
	while (map[row][column] == 'O') {
		row += diffRow;
		column += diffColumn;
	}
	if (map[row][column] == '.') {
		map[playerRow][playerColumn] = '.';
		map[row][column] = 'O';
		playerRow += diffRow;
		playerColumn += diffColumn;
		map[playerRow][playerColumn] = '@';
	}
	return {playerRow, playerColumn};
}

void printMap(const std::vector<std::string>& map) {
	for (const std::string& line : map) {
		std::cout << line << "\n";
	}
}

int f1(std::istream& in) {
	std::vector<std::string> map;
	size_t playerRow = 0, playerColumn = 0;
	size_t row = 0;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) break;
		map.push_back(line);
		size_t atColumn = line.find('@');
		if (atColumn != std::string::npos) {
			playerRow = row;
			playerColumn = atColumn;
		}
		row++;
	}

	while (std::getline(in, line)) {
		for (char c : line) {
			std::tie(playerRow, playerColumn) = attemptStep(map, playerRow, playerColumn, c);
		}
	}

	size_t sum = 0;
	for (size_t row = 0; row < map.size(); row++) {
		for (size_t column = 0; column < map.size(); column++) {
			if (map[row][column] == 'O') {
				sum += 100 * row + column;
			}
		}
	}

	std::cout << sum << "\n";

	return 0;
}

int f2(std::istream& in) {
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}