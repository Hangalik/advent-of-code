#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "input_selector.h"
#include "matricies.h"

struct BeaconPosition {
	int x, y, z;
};

struct BeaconHasher {
	size_t operator()(const BeaconPosition& bp) const {
		size_t value = 31 * bp.x + bp.y;
		value *= 31;
		value += bp.z;
		return value;
	}
};

struct BeaconEqualTo {
	bool operator()(const BeaconPosition& bp1, const BeaconPosition& bp2) const {
		return bp1.x == bp2.x && bp1.y == bp2.y && bp1.z == bp2.z;
	}
};

BeaconPosition orientPosition(const BeaconPosition& bp, const int (&m)[3][3]) {
	int x = bp.x * m[0][0] + bp.y * m[1][0] + bp.z * m[2][0];
	int y = bp.x * m[0][1] + bp.y * m[1][1] + bp.z * m[2][1];
	int z = bp.x * m[0][2] + bp.y * m[1][2] + bp.z * m[2][2];
	return {x, y, z};
}

BeaconPosition slidePosition(const BeaconPosition& bp, const int (&m)[3]) {
	return {bp.x + m[0], bp.y + m[1], bp.z + m[2]};
}

class Scanner {
public:
	void addBeacon(const BeaconPosition& bp) {
		beacons.push_back(bp);
	}

	const std::vector<BeaconPosition>& getBeacons() const {
		return beacons;
	}

	void printBeacons() const {
		for (const BeaconPosition& bp : beacons) {
			std::cout << bp.x << ";" << bp.y << ";" << bp.z << std::endl;
		}
	}

private:
	std::vector<BeaconPosition> beacons;
};

int countOfOverlappingBeacons(const std::vector<BeaconPosition>& beacons1, const std::vector<BeaconPosition>& beacons2) {
	std::unordered_map<BeaconPosition, int, BeaconHasher, BeaconEqualTo> beaconCounts;
	for (const BeaconPosition& bp : beacons1) {
		beaconCounts[bp]++;
	}
	for (const BeaconPosition& bp : beacons2) {
		beaconCounts[bp]++;
	}
	int overlaps = 0;
	for (const auto& count : beaconCounts) {
		if (count.second > 1) {
			overlaps++;
		}
	}
	return overlaps;
}

static constexpr int enoughCommonBeacons = 12;

bool haveEnoughOverlappingBeacons(const std::vector<BeaconPosition>& beacons1, const std::vector<BeaconPosition>& beacons2) {
	for (const BeaconPosition& bp1 : beacons1) {
		for (const BeaconPosition& bp2 : beacons2) {
			int slideVector[3] = {bp2.x - bp1.x, bp2.y - bp1.y, bp2.z - bp1.z};
			std::vector<BeaconPosition> slidPositions;
			for (const BeaconPosition& bp : beacons1) {
				slidPositions.push_back(slidePosition(bp, slideVector));
			}
			int overlaps = countOfOverlappingBeacons(slidPositions, beacons2);
			if (overlaps >= enoughCommonBeacons) {
				return true;
			}
		}
	}
	return false;
}

bool haveEnoughCommonBeacons(const Scanner& scanner1, const Scanner& scanner2) {
	for (const auto& matrix : orientingMatricies) {
		std::vector<BeaconPosition> orientedBeacons;
		for (const BeaconPosition& position : scanner1.getBeacons()) {
			orientedBeacons.push_back(orientPosition(position, matrix));
		}
		if (haveEnoughOverlappingBeacons(orientedBeacons, scanner2.getBeacons())) {
			return true;
		}
	}
	return false;
}

int f1(std::istream& in) {
	std::vector<Scanner> scanners;
	std::string line;
	while (std::getline(in, line, '\n')) {
		if (line.empty()) {
			continue;
		}
		if (line.find("scanner") != std::string::npos) {
			scanners.push_back(Scanner{});
			continue;
		}
		size_t firstEnd = line.find(',');
		size_t secondEnd = line.find(',', firstEnd + 1);
		int x = std::stoi(line.substr(0, firstEnd));
		int y = std::stoi(line.substr(firstEnd + 1, secondEnd - firstEnd - 1));
		int z = std::stoi(line.substr(secondEnd + 1));
		scanners.back().addBeacon({x, y, z});
	}

	for (size_t i = 0; i < scanners.size(); i++) {
		for (size_t j = i + 1; j < scanners.size(); j++) {
			bool b = haveEnoughCommonBeacons(scanners[i], scanners[j]);
			if (b) {
				std::cout << "Neighbours: " << i << " - " << j << std::endl;
			}
		}
	}

	return 0;
}

int f2(std::istream& ) {

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