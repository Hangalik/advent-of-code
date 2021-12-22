#include <iostream>
#include <string>
#include <utility>
#include <array>
#include <vector>

#include "input_selector.h"

std::pair<int, int> getValuePair(const std::string& valueStr) {
	size_t seperate = valueStr.find("..");
	std::string firstStr = valueStr.substr(2, seperate);
	std::string secondStr = valueStr.substr(seperate + 2);
	int firstValue = std::stoi(firstStr);
	int secondValue = std::stoi(secondStr);
	return {firstValue, secondValue};
}

bool isOutsideRange(int val) {
	return val < -50 || val > 50;
}

template <typename... Ts>
bool anyOutsideRange(Ts... vals) {
	return (isOutsideRange(vals) || ...);
}

template <typename T, size_t... sizes>
class Array;

template <typename T, size_t firstSize, size_t... sizes>
class Array<T, firstSize, sizes...> : public std::array<Array<T, sizes...>, firstSize> {};

template <typename T, size_t size>
class Array<T, size> : public std::array<T, size> {};

int f1(std::istream& in) {
	Array<bool, 101, 101, 101> reactorCore{};
	std::string line;
	while (in >> line) {
		if (line.front() == '#') {
			std::getline(in, line, '\n');
			continue;
		}
		in.ignore();
		bool turnOn = line == "on";
		std::getline(in, line, ',');
		auto [x1, x2] = getValuePair(line);
		std::getline(in, line, ',');
		auto [y1, y2] = getValuePair(line);
		std::getline(in, line, '\n');
		auto [z1, z2] = getValuePair(line);

		if (anyOutsideRange(x1, x2, y1, y2, z1, z2)) {
			continue;
		}
		for (int i = x1; i <= x2; i++) {
			int x = i + 50;
			for (int j = y1; j <= y2; j++) {
				int y = j + 50;
				for (int k = z1; k <= z2; k++) {
					int z = k + 50;
					reactorCore[x][y][z] = turnOn;
				}
			}
		}
	}

	int onCount = 0;
	for (const auto& plane : reactorCore) {
		for (const auto& row : plane) {
			for (bool cube : row) {
				onCount += cube;
			}
		}
	}

	std::cout << onCount << std::endl;

	return 0;
}

class Cuboid {
public:
	Cuboid(int x1, int x2, int y1, int y2, int z1, int z2) :
		x1(x1), x2(x2),
		y1(y1), y2(y2),
		z1(z1), z2(z2)
	{}

	void print() const {
		std::cout << "x=" << x1 << ".." << x2 << ",y=" << y1 << ".." << y2 << ",z=" << z1 << ".." << z2 << std::endl;
	}

	long long getVolume() const {
		long long volume = 1LL;
		volume *= x2 - x1 + 1;
		volume *= y2 - y1 + 1;
		volume *= z2 - z1 + 1;
		return volume;
	}

	bool doesIntersect(const Cuboid& other) const {
		bool cond1 = x2 < other.x1;
		bool cond2 = other.x2 < x1;
		bool cond3 = y2 < other.y1;
		bool cond4 = other.y2 < y1;
		bool cond5 = z2 < other.z1;
		bool cond6 = other.z2 < z1;
		return !(cond1 || cond2 || cond3 || cond4 || cond5 || cond6);
	}

	Cuboid getIntersect(const Cuboid& other) const {
		int newX1 = std::max(x1, other.x1);
		int newY1 = std::max(y1, other.y1);
		int newZ1 = std::max(z1, other.z1);
		int newX2 = std::min(x2, other.x2);
		int newY2 = std::min(y2, other.y2);
		int newZ2 = std::min(z2, other.z2);
		return {newX1, newX2, newY1, newY2, newZ1, newZ2};
	}

	void divideOn(const Cuboid& other, std::vector<Cuboid>& result) const {
		if (x1 < other.x1) {
			result.emplace_back(x1, other.x1 - 1, y1, y2, z1, z2);
		}
		int leftX1 = std::max(x1, other.x1);
		if (x2 > other.x2) {
			result.emplace_back(other.x2 + 1, x2, y1, y2, z1, z2);
		}
		int leftX2 = std::min(x2, other.x2);

		if (y1 < other.y1) {
			result.emplace_back(leftX1, leftX2, y1, other.y1 - 1, z1, z2);
		}
		int leftY1 = std::max(y1, other.y1);
		if (y2 > other.y2) {
			result.emplace_back(leftX1, leftX2, other.y2 + 1, y2, z1, z2);
		}
		int leftY2 = std::min(y2, other.y2);

		if (z1 < other.z1) {
			result.emplace_back(leftX1, leftX2, leftY1, leftY2, z1, other.z1 - 1);
		}
		if (z2 > other.z2) {
			result.emplace_back(leftX1, leftX2, leftY1, leftY2, other.z2 + 1, z2);
		}
	}

private:
	int x1, x2;
	int y1, y2;
	int z1, z2;
};

void printCuboids(const std::vector<Cuboid>& areas, const char* str) {
	std::cout << str << areas.size() << std::endl;
	for (const Cuboid& area : areas) {
		area.print();
	}
	std::cout << std::endl;
}

int f2(std::istream& in) {
	std::vector<Cuboid> areas;
	std::string line;
	while (in >> line) {
		if (line.front() == '#') {
			std::getline(in, line, '\n');
			continue;
		}
		in.ignore();
		bool turnOn = line == "on";
		std::getline(in, line, ',');
		auto [x1, x2] = getValuePair(line);
		std::getline(in, line, ',');
		auto [y1, y2] = getValuePair(line);
		std::getline(in, line, '\n');
		auto [z1, z2] = getValuePair(line);

		std::vector<Cuboid> cuboids{{x1, x2, y1, y2, z1, z2}};
		std::vector<Cuboid> newAreas;
		for (const Cuboid& area : areas) {
			bool areaIntersectsAny = false;
			std::vector<Cuboid> newCuboids;
			for (const Cuboid& cuboidPart : cuboids) {
				if (area.doesIntersect(cuboidPart)) {
					areaIntersectsAny = true;
					Cuboid intersectCuboid = area.getIntersect(cuboidPart);
					area.divideOn(intersectCuboid, newAreas);
					cuboidPart.divideOn(intersectCuboid, newCuboids);
					if (turnOn) {
						newAreas.push_back(intersectCuboid);
					}
				} else {
					newCuboids.push_back(cuboidPart);
				}
			}
			if (!areaIntersectsAny) {
				newAreas.push_back(area);
			}
			cuboids = std::move(newCuboids);
			//printCuboids(cuboids, "cuboids: ");
		}
		if (turnOn) {
			newAreas.insert(newAreas.end(), cuboids.begin(), cuboids.end());
		}
		areas = std::move(newAreas);

		//printCuboids(areas, "areas: ");
	};

	//printCuboids(areas, "areas: ");
	long long onCount = 0;
	for (const Cuboid& area : areas) {
		onCount += area.getVolume();
	}
	std::cout << onCount << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
	return 1;
}
