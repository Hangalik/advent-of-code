#include <iostream>
#include <vector>
#include <algorithm>

#include "input_selector.h"

struct Garden {
	std::vector<std::string> plants;
	std::vector<std::vector<bool>> visited;
};

Garden readGarden(std::istream& in) {
	Garden garden;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		garden.plants.push_back(line);
		garden.visited.emplace_back(line.size(), false);
	}
	return garden;
}

struct RegionStats {
	int area;
	int perimeter;
};

RegionStats& operator+=(RegionStats& a, const RegionStats& b) {
	a.area += b.area;
	a.perimeter += b.perimeter;
	return a;
}

RegionStats calculateRegionStats(Garden& garden, size_t row, size_t column) {
	if (garden.visited[row][column]) {
		return {0, 0};
	}
	garden.visited[row][column] = true;

	char regionPlant = garden.plants[row][column];
	RegionStats regionStats(1, 4);
	if (row > 0 && garden.plants[row - 1][column] == regionPlant) {
		regionStats += calculateRegionStats(garden, row - 1, column);
		regionStats += {0, -1};
	}
	if (column > 0 && garden.plants[row][column - 1] == regionPlant) {
		regionStats += calculateRegionStats(garden, row, column - 1);
		regionStats += {0, -1};
	}
	if (row < garden.plants.size() - 1 && garden.plants[row + 1][column] == regionPlant) {
		regionStats += calculateRegionStats(garden, row + 1, column);
		regionStats += {0, -1};
	}
	if (column < garden.plants[row].size() - 1 && garden.plants[row][column + 1] == regionPlant) {
		regionStats += calculateRegionStats(garden, row, column + 1);
		regionStats += {0, -1};
	}
	return regionStats;
}

int calculatePrice(Garden& garden) {
	int price = 0;
	for (size_t i = 0; i < garden.plants.size(); i++) {
		for (size_t j = 0; j < garden.plants[i].size(); j++) {
			RegionStats stats = calculateRegionStats(garden, i, j);
			price += stats.area * stats.perimeter;
		}
	}
	return price;
}

int f1(std::istream& in) {
	Garden garden = readGarden(in);
	int price = calculatePrice(garden);
	std::cout << price << "\n";

	return 0;
}

struct Fence {
	size_t equalDimension; // row for horizontal fences, column for vertical fences
	size_t increasingDimension; // column for horizontal fences, row for vertical fences
};

struct Fences {
	std::vector<Fence> northFences;
	std::vector<Fence> eastFences;
	std::vector<Fence> southFences;
	std::vector<Fence> westFences;
};

int walkRegion(Garden& garden, size_t row, size_t column, Fences& fences) {
	if (garden.visited[row][column]) {
		return 0;
	}
	garden.visited[row][column] = true;

	char regionPlant = garden.plants[row][column];
	int area = 1;
	if (row > 0 && garden.plants[row - 1][column] == regionPlant) {
		area += walkRegion(garden, row - 1, column, fences);
	} else {
		fences.southFences.emplace_back(row, column);
	}
	if (column > 0 && garden.plants[row][column - 1] == regionPlant) {
		area += walkRegion(garden, row, column - 1, fences);
	} else {
		fences.eastFences.emplace_back(column, row);
	}
	if (row < garden.plants.size() - 1 && garden.plants[row + 1][column] == regionPlant) {
		area += walkRegion(garden, row + 1, column, fences);
	} else {
		fences.northFences.emplace_back(row + 1, column);
	}
	if (column < garden.plants[row].size() - 1 && garden.plants[row][column + 1] == regionPlant) {
		area += walkRegion(garden, row, column + 1, fences);
	} else {
		fences.westFences.emplace_back(column + 1, row);
	}
	return area;
}

int calculateSides(std::vector<Fence>& fences) {
	std::ranges::sort(fences, [](const Fence& a, const Fence& b) {
		if (a.equalDimension != b.equalDimension) {
			return a.equalDimension < b.equalDimension;
		}
		return a.increasingDimension < b.increasingDimension;
	});

	int sides = 0;
	size_t i = 0;
	while (i < fences.size()) {
		size_t staysEqual = fences[i].equalDimension;
		size_t increases = fences[i].increasingDimension;
		sides++;
		while (++i < fences.size()) {
			if (fences[i].equalDimension != staysEqual) {
				break;
			}
			if (fences[i].increasingDimension != ++increases) {
				break;
			}
		}
	}
	return sides;
}

int calculateDiscountedPriceForRegion(Garden& garden, size_t row, size_t column) {
	if (garden.visited[row][column]) {
		return 0;
	}
	Fences fences;
	int area = walkRegion(garden, row, column, fences);
	int sides = 0;
	sides += calculateSides(fences.northFences);
	sides += calculateSides(fences.southFences);
	sides += calculateSides(fences.eastFences);
	sides += calculateSides(fences.westFences);
	return sides * area;
}

int calculateDiscountedPrice(Garden& garden) {
	int price = 0;
	for (size_t i = 0; i < garden.plants.size(); i++) {
		for (size_t j = 0; j < garden.plants[i].size(); j++) {
			price += calculateDiscountedPriceForRegion(garden, i, j);
		}
	}
	return price;
}

int f2(std::istream& in) {
	Garden garden = readGarden(in);
	int price = calculateDiscountedPrice(garden);
	std::cout << price << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}