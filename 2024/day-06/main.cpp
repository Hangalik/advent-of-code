#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <utility>

#include "input_selector.h"

enum class Direction {
	UP, RIGHT, DOWN, LEFT
};

int f1(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	size_t row = 0, column = 0;
	Direction dir = Direction::UP;
	while (std::getline(in, line)) {
		size_t i = line.find('^');
		if (i != std::string::npos) {
			row = lines.size();
			column = i;
		}
		lines.push_back(line);
	}

	lines[row][column] = 'X';

	while (true) {
		if (dir == Direction::UP) {
			if (row == 0) {
				break;
			} else if (lines[row - 1][column] == '#') {
				dir = Direction::RIGHT;
			} else {
				row--;
			}
		} else if (dir == Direction::RIGHT) {
			if (column == lines[row].size() - 1) {
				break;
			} else if (lines[row][column + 1] == '#') {
				dir = Direction::DOWN;
			} else {
				column++;
			}
		} else if (dir == Direction::DOWN) {
			if (row == lines.size() - 1) {
				break;
			} else if (lines[row + 1][column] == '#') {
				dir = Direction::LEFT;
			} else {
				row++;
			}
		} else if (dir == Direction::LEFT) {
			if (column == 0) {
				break;
			} else if (lines[row][column - 1] == '#') {
				dir = Direction::UP;
			} else {
				column--;
			}
		}
		lines[row][column] = 'X';
	}

	size_t sum = 0;
	for (const std::string& line : lines) {
		sum += std::ranges::count(line, 'X');
	}

	std::cout << sum << '\n';

	return 0;
}

class DirectionSet {
public:
	bool hasDirection(Direction dir) const {
		size_t bit = std::to_underlying(dir);
		return directions.test(bit);
	}

	void addDirection(Direction dir) {
		size_t bit = std::to_underlying(dir);
		directions.set(bit);
	}

private:
	std::bitset<4> directions;
};

bool hasCircle(const std::vector<std::string>& lines, size_t row, size_t column, std::vector<std::vector<DirectionSet>>& visitedCells) {
	Direction dir = Direction::UP;
	while (true) {
		if (visitedCells[row][column].hasDirection(dir)) {
			return true;
		}
		visitedCells[row][column].addDirection(dir);
		if (dir == Direction::UP) {
			if (row == 0) {
				break;
			} else if (lines[row - 1][column] == '#') {
				dir = Direction::RIGHT;
			} else {
				row--;
			}
		} else if (dir == Direction::RIGHT) {
			if (column == lines[row].size() - 1) {
				break;
			} else if (lines[row][column + 1] == '#') {
				dir = Direction::DOWN;
			} else {
				column++;
			}
		} else if (dir == Direction::DOWN) {
			if (row == lines.size() - 1) {
				break;
			} else if (lines[row + 1][column] == '#') {
				dir = Direction::LEFT;
			} else {
				row++;
			}
		} else if (dir == Direction::LEFT) {
			if (column == 0) {
				break;
			} else if (lines[row][column - 1] == '#') {
				dir = Direction::UP;
			} else {
				column--;
			}
		}
	}
	return false;
}

void resetVisitedCells(std::vector<std::vector<DirectionSet>>& visitedCells) {
	for (size_t i = 0; i < visitedCells.size(); i++) {
		visitedCells[i].assign(visitedCells[i].size(), DirectionSet{});
	}
}

int f2(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	size_t row = 0, column = 0;
	while (std::getline(in, line)) {
		size_t i = line.find('^');
		if (i != std::string::npos) {
			row = lines.size();
			column = i;
		}
		lines.push_back(line);
	}

	std::vector<std::vector<DirectionSet>> visitedCells(lines.size(), std::vector<DirectionSet>(lines[0].size()));

	int count = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			if (lines[i][j] == '.') {
				resetVisitedCells(visitedCells);
				lines[i][j] = '#';
				if (hasCircle(lines, row, column, visitedCells)) {
					count++;
				}
				lines[i][j] = '.';
			}
		}
	}

	std::cout << count << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}