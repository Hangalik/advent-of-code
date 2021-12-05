#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

#include "input_selector.h"

std::pair<int, int> readCoords(std::istream& in) {
	int x, y;
	in >> x;
	in.ignore();
	in >> y;
	return {x, y};
}

template <typename T>
void resizeIfSmaller(std::vector<T>& v, size_t size) {
	size++;
	if (v.size() < size) {
		v.reserve(size);
		v.resize(size);
	}
}

void printSeaFloor(std::vector<std::vector<int>> seaFloor) {
	for (const std::vector<int>& column : seaFloor) {
		for (int row : column) {
			if (row == 0) {
				std::cout << '.';
			} else {
				std::cout << row;
			}
		}
		std::cout << std::endl;
	}
}

int f1(std::istream& in) {

	std::vector<std::vector<int>> seaFloor;
	while (true) {
		auto [x1, y1] = readCoords(in);
		in.ignore(3);
		auto [x2, y2] = readCoords(in);
		if (in.fail()) {
			break;
		}
		if (x1 == x2) {
			resizeIfSmaller(seaFloor, x1);
			resizeIfSmaller(seaFloor[x1], std::max(y1, y2));
			for (int i = std::min(y1, y2); i <= std::max(y1, y2); i++) {
				seaFloor[x1][i]++;
			}
		}
		if (y1 == y2) {
			resizeIfSmaller(seaFloor, std::max(x1, x2));
			for (int i = std::min(x1, x2); i <= std::max(x1, x2); i++) {
				resizeIfSmaller(seaFloor[i], y1);
				seaFloor[i][y1]++;
			}
		}
	}

	int dangerousTileCount = 0;
	for (const std::vector<int>& column : seaFloor) {
		for (int row : column) {
			if (row >= 2) {
				dangerousTileCount++;
			}
		}
	}
	std::cout << dangerousTileCount << std::endl;

	return 0;
}

int sgn(int a) {
	if (a == 0) return 0;
	if (a < 0) return -1;
	return 1;
}

int f2(std::istream& in) {
	std::vector<std::vector<int>> seaFloor;
	while (true) {
		auto [x1, y1] = readCoords(in);
		in.ignore(3);
		auto [x2, y2] = readCoords(in);
		if (in.fail()) {
			break;
		}
		resizeIfSmaller(seaFloor, std::max(x1, x2));
		int incX = x2 - x1;
		int incY = y2 - y1;
		int count = std::max(std::abs(incX), std::abs(incY)) + 1;
		for (int i = 0; i < count; i++) {
			int x = x1 + i * sgn(incX);
			int y = y1 + i * sgn(incY);
			resizeIfSmaller(seaFloor[x], y);
			seaFloor[x][y]++;
		}
	}

	int dangerousTileCount = 0;
	for (const std::vector<int>& column : seaFloor) {
		for (int row : column) {
			if (row >= 2) {
				dangerousTileCount++;
			}
		}
	}
	std::cout << dangerousTileCount << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::length_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return 1;
}