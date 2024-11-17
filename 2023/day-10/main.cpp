#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "input_selector.h"

enum class Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

Direction getStartingDirection(const std::vector<std::string>& map, size_t row, size_t col) {
	if (row > 0) {
		char c = map[row - 1][col];
		if (c == '|' || c == 'F' || c == '7') {
			return Direction::NORTH;
		}
	}
	if (row < map.size()) {
		char c = map[row + 1][col];
		if (c == '|' || c == 'J' || c == 'L') {
			return Direction::SOUTH;
		}
	}

	if (col > 0) {
		char c = map[row][col - 1];
		if (c == '-' || c == 'L' || c == 'F') {
			return Direction::WEST;
		}
	}
	if (col < map[row].size()) {
		char c = map[row][col + 1];
		if (c == '-' || c == 'J' || c == '7') {
			return Direction::EAST;
		}
	}

	std::unreachable();
}

Direction nextDirection(const std::vector<std::string>& map, size_t& row, size_t& col, Direction currentDirection) {
	switch (currentDirection) {
	case Direction::NORTH: {
		row--;
		char c = map[row][col];
		if (c == '|') return Direction::NORTH;
		if (c == '7') return Direction::WEST;
		if (c == 'F') return Direction::EAST;
		break;
	}
	case Direction::EAST: {
		col++;
		char c = map[row][col];
		if (c == '-') return Direction::EAST;
		if (c == '7') return Direction::SOUTH;
		if (c == 'J') return Direction::NORTH;
		break;
	}
	case Direction::SOUTH: {
		row++;
		char c = map[row][col];
		if (c == '|') return Direction::SOUTH;
		if (c == 'J') return Direction::WEST;
		if (c == 'L') return Direction::EAST;
		break;
	}
	case Direction::WEST: {
		col--;
		char c = map[row][col];
		if (c == '-') return Direction::WEST;
		if (c == 'F') return Direction::SOUTH;
		if (c == 'L') return Direction::NORTH;
		break;
	}
	}
	std::unreachable();
}

int f1(std::istream& in) {
	std::vector<std::string> map;
	std::string line;
	size_t row = 0;
	size_t col = 0;
	while (std::getline(in, line)) {
		map.push_back(line);
		size_t sPos = line.find('S');
		if (sPos != std::string::npos) {
			row = map.size() - 1;
			col = sPos;
		}
	}

	Direction direction = getStartingDirection(map, row, col);

	int steps = 0;
	do {
		steps++;
		direction = nextDirection(map, row, col, direction);
	} while (map[row][col] != 'S');

	std::cout << steps / 2 << "\n";

	return 0;
}

int f2(std::istream& in) {
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}