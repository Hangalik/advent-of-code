#include <iostream>
#include <sstream>
#include <vector>

using grid_t = std::vector<std::vector<bool>>;

constexpr int BUFFER_SIZE = 16;

grid_t createEmpty(int x) {
	grid_t retVal{};
	for (int i = 0; i < x; i++) {
		retVal.push_back(std::vector<bool>(x));
	}
	return retVal;
}

grid_t whereIsWire(const std::string& wireString) {
	std::stringstream sstream{ wireString };
	char way[BUFFER_SIZE];
	grid_t wire{ createEmpty(50000) };
	const int middle{ 25000 };
	int x{ 0 };
	int y{ 0 };
	while (sstream.getline(way, BUFFER_SIZE, ',')) {
		const int number{ std::stoi(way + 1) };
		switch (way[0]) {
		case 'R':
			for (int i = 0; i < number; i++) {
				x++;
				wire.at(middle + x).at(middle + y) = true;
			}
			break;
		case 'L':
			for (int i = 0; i < number; i++) {
				x--;
				wire.at(middle + x).at(middle + y) = true;
			}
			break;
		case 'U':
			for (int i = 0; i < number; i++) {
				y++;
				wire.at(middle + x).at(middle + y) = true;
			}
			break;
		case 'D':
			for (int i = 0; i < number; i++) {
				y--;
				wire.at(middle + x).at(middle + y) = true;
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
			result[i][j] = first[i][j] && second[i][j];
		}
	}
	return result;
}

unsigned int minimumManhattanDistance(const grid_t& grid) {
	const auto width{ grid.size() };
	const auto height{ grid[0].size() };
	const auto middle{ width / 2 };

	auto minimumDistance{ std::numeric_limits<int>::max() };
	for (unsigned int i = 0u; i < width; i++) {
		const int xDistance{ std::abs(static_cast<int>(i) - static_cast<int>(middle)) };
		for (unsigned int j = 0u; j < height; j++) {
			const int yDistance{ std::abs(static_cast<int>(j) - static_cast<int>(middle)) };
			if (grid[i][j] && xDistance + yDistance < minimumDistance) {
				minimumDistance = xDistance + yDistance;
			}
		}
	}
	return minimumDistance;
}

int main() {

	std::string line;
	std::cin >> line;
	grid_t firstWire{ whereIsWire(line) };
	std::cin >> line;
	grid_t secondWire{ whereIsWire(line) };

	grid_t intersects{ gridAnd(firstWire, secondWire) };

	auto minDistance{ minimumManhattanDistance(intersects) };

	std::cout << "Minimum manhattan distance is " << minDistance << std::endl;

	return 0;
}

