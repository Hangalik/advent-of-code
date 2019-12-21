#include <iostream>
#include <sstream>
#include <vector>

using grid_t = std::vector<std::vector<int>>;

constexpr int BUFFER_SIZE = 16;

grid_t createEmpty(int x) {
	grid_t retVal{};
	for (int i = 0; i < x; i++) {
		retVal.push_back(std::vector<int>(x, -1));
	}
	return retVal;
}

grid_t whereIsWire(const std::string& wireString) {
	std::stringstream sstream{ wireString };
	char way[BUFFER_SIZE];
	grid_t wire{ createEmpty(30000) };
	const int middle{ 15000 };
	int x{ 0 };
	int y{ 0 };
	int steps{ 1 };
	while (sstream.getline(way, BUFFER_SIZE, ',')) {
		const int number{ std::stoi(way + 1) };
		switch (way[0]) {
		case 'R':
			for (int i = 0; i < number; i++) {
				x++;
				wire.at(middle + x).at(middle + y) = steps++;
			}
			break;
		case 'L':
			for (int i = 0; i < number; i++) {
				x--;
				wire.at(middle + x).at(middle + y) = steps++;
			}
			break;
		case 'U':
			for (int i = 0; i < number; i++) {
				y++;
				wire.at(middle + x).at(middle + y) = steps++;
			}
			break;
		case 'D':
			for (int i = 0; i < number; i++) {
				y--;
				wire.at(middle + x).at(middle + y) = steps++;
			}
			break;
		default:
			throw "Incorrect character: " + way[0];
		}
	}
	return wire;
}

grid_t gridAnd(const grid_t& first, const grid_t& second) {
	const auto width{ first.size() };
	const auto height{ first[0].size() };
	grid_t result{ createEmpty(width) };
	for (auto i{ 0u }; i < width; i++) {
		for (auto j{ 0u }; j < height; j ++) {
			if (first[i][j] > 0 && second[i][j] > 0) {
				result[i][j] = first[i][j] + second[i][j];
			}
		}
	}
	return result;
}

unsigned int minimumManhattanDistance(const grid_t& grid) {
	const auto width{ grid.size() };
	const auto height{ grid[0].size() };

	auto minimumStepSum{ std::numeric_limits<int>::max() };
	for (unsigned int i = 0u; i < width; i++) {
		for (unsigned int j = 0u; j < height; j++) {
			if (grid[i][j] > 0 && minimumStepSum > grid[i][j]) {
				minimumStepSum = grid[i][j];
			}
		}
	}
	return minimumStepSum;
}

int main() {

	std::string line;
	std::cin >> line;
	grid_t firstWire{ whereIsWire(line) };
	std::cin >> line;
	grid_t secondWire{ whereIsWire(line) };

	grid_t intersects{ gridAnd(firstWire, secondWire) };

	auto minDistance{ minimumManhattanDistance(intersects) };

	std::cout << "Minimum steps is " << minDistance << std::endl;

	return 0;
}

