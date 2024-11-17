#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <ranges>

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

char getPipeOfStart(const std::vector<std::string>& map, size_t row, size_t col) {
	bool top = false;
	if (row > 0) {
		char c = map[row - 1][col];
		if (c == '|' || c == 'F' || c == '7') {
			top = true;
		}
	}
	bool bottom = false;
	if (row < map.size()) {
		char c = map[row + 1][col];
		if (c == '|' || c == 'J' || c == 'L') {
			return bottom = true;
		}
	}

	bool left = false;
	if (col > 0) {
		char c = map[row][col - 1];
		if (c == '-' || c == 'L' || c == 'F') {
			left = true;
		}
	}
	bool right = false;
	if (col < map[row].size()) {
		char c = map[row][col + 1];
		if (c == '-' || c == 'J' || c == '7') {
			right = true;
		}
	}

	if (top && right) return 'L';
	if (top && bottom) return '|';
	if (top && left) return 'J';
	if (right && bottom) return 'F';
	if (right && left) return '-';
	if (bottom && left) return '7';
	std::unreachable();
}

int f2(std::istream& in) {
	std::vector<std::string> map;
	std::vector<std::vector<bool>> mainLoop;
	std::string line;
	size_t row = 0;
	size_t col = 0;
	while (std::getline(in, line)) {
		map.push_back(line);
		mainLoop.emplace_back(line.size(), false);
		size_t sPos = line.find('S');
		if (sPos != std::string::npos) {
			row = map.size() - 1;
			col = sPos;
		}
	}

	Direction direction = getStartingDirection(map, row, col);

	do {
		mainLoop[row][col] = true;
		direction = nextDirection(map, row, col, direction);
	} while (map[row][col] != 'S');

	map[row][col] = getPipeOfStart(map, row, col);

	int insideCount = 0;
	for (const auto& [mapLine, loopLine] : std::views::zip(map, mainLoop)) {
		bool inside = false;
		char pipeThatTurns = ' ';
		for (const auto& [mapP, loopP] : std::views::zip(mapLine, loopLine)) {
			if (!loopP) {
				if (inside) insideCount++;
			} else if (mapP == '|') {
				inside = !inside;
			} else if (mapP == '-') {
				continue;
			} else if (pipeThatTurns == ' ') {
				if (mapP == 'F') pipeThatTurns = 'J';
				if (mapP == 'L') pipeThatTurns = '7';
			} else if (mapP == pipeThatTurns) {
				inside = !inside;
				pipeThatTurns = ' ';
			} else {
				pipeThatTurns = ' ';
			}
		}
	}

	std::cout << insideCount << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}