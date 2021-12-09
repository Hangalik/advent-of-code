#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <list>

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

class FloorLevel {
public:
	explicit FloorLevel(int level) : level(level) {}

	void addNeighbour(FloorLevel* newNeighbour) {
		if (neighbours.size() >= 4u) {
			throw std::length_error("FloorLevel's neighbour count cannot exceed 4");
		}
		neighbours.insert(newNeighbour);
	}

	const std::unordered_set<FloorLevel*>& getNeighbours() const {
		return neighbours;
	}

	bool isLowPoint() const {
		for (FloorLevel* neighbor : neighbours) {
			if (neighbor->level <= level) {
				return false;
			}
		}
		return true;
	}

	const int level;
private:
	std::unordered_set<FloorLevel*> neighbours;
};

void linkNeighbours(FloorLevel& lhs, FloorLevel& rhs) {
	lhs.addNeighbour(&rhs);
	rhs.addNeighbour(&lhs);
}

int basinSize(const FloorLevel& floorLevel) {
	int size = 0;
	std::unordered_set<const FloorLevel*> visitedFloors;
	std::unordered_set<const FloorLevel*> activeFloors;
	activeFloors.insert(&floorLevel);
	while (!activeFloors.empty()) {
		auto current = activeFloors.begin();
		const FloorLevel* currentFloor = *current;
		visitedFloors.insert(activeFloors.extract(current));
		size++;

		for (const FloorLevel* neighbour : currentFloor->getNeighbours()) {
			if (neighbour->level != 9 && !visitedFloors.contains(neighbour)) {
				activeFloors.insert(neighbour);
			}
		}
	}
	return size;
}

int f2(std::istream& in) {
	// Needs to be a container that doesn't copy around :P
	std::list<std::list<FloorLevel>> floorLevels;
	std::string line;
	while (in >> line) {
		std::list<FloorLevel> lineLevels;
		for (size_t i = 0; i < line.size(); i++) {
			int point = std::stoi(line.substr(i, 1));
			lineLevels.emplace_back(point);

			if (i > 0u) {
				auto lastIt = lineLevels.rbegin();
				FloorLevel& last = *lastIt;
				lastIt++;
				FloorLevel& beforeLast = *lastIt;
				linkNeighbours(last, beforeLast);
			}
			if (!floorLevels.empty()) {
				auto lastFloor = floorLevels.back().begin();
				std::advance(lastFloor, i);
				linkNeighbours(lineLevels.back(), *lastFloor);
			}
		}
		floorLevels.push_back(std::move(lineLevels));
	}

	std::multiset<int, std::less<int>> basinSizes;
	for (const std::list<FloorLevel>& floors : floorLevels) {
		for (const FloorLevel& level : floors) {
			if (level.isLowPoint()) {
				int sizeOfBasin = basinSize(level);
				//std::cout << "Low point basin size: " << sizeOfBasin << std::endl;
				basinSizes.insert(sizeOfBasin);
			}
		}
	}

	if (basinSizes.size() < 3u) {
		std::cerr << "There is no 3 basins in the input" << std::endl;
		return 1;
	}
	auto bigEnd = basinSizes.rbegin();
	int most = *bigEnd;
	bigEnd++;
	int secondMost = *bigEnd;
	bigEnd++;
	int thirdMost = *bigEnd;
	std::cout << most << " * " << secondMost << " * " << thirdMost << " = " << most * secondMost * thirdMost << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	}
}
