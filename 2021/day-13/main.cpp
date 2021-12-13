#include <iostream>
#include <regex>
#include <vector>
#include <unordered_set>
#include <string>

#include "input_selector.h"

struct PairHasher {
	size_t operator()(const std::pair<int, int>& v) const {
		return 31 * v.first + v.second;
	}
};

int f1(std::istream& in) {
	std::regex foldLine("fold along ([xy])=(\\d*)");
	std::string line;
	std::unordered_set<std::pair<int, int>, PairHasher> points;
	while (std::getline(in, line, '\n')) {
		if (line.empty()) {
			continue;
		}
		std::smatch result;
		if (std::regex_match(line, result, foldLine)) {
			int foldValue = std::stoi(result.str(2));
			std::unordered_set<std::pair<int, int>, PairHasher> newPoints;
			if (result.str(1) == "x") {
				for (auto& [x, y] : points) {
					std::pair<int, int> newPoint(x, y);
					if (x > foldValue) {
						newPoint.first = 2 * foldValue - x;
					}
					newPoints.insert(newPoint);
				}
			} else {
				for (auto& [x, y] : points) {
					std::pair<int, int> newPoint(x, y);
					if (y > foldValue) {
						newPoint.second = 2 * foldValue - y;
					}
					newPoints.insert(newPoint);
				}
			}
			points.swap(newPoints);
			break;
		}
		size_t split = line.find(',');
		int x = std::stoi(line.substr(0, split));
		int y = std::stoi(line.substr(split + 1));
		points.emplace(x, y);
	}

	std::cout << points.size() << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::regex foldLine("fold along ([xy])=(\\d*)");
	std::string line;
	std::unordered_set<std::pair<int, int>, PairHasher> points;
	while (std::getline(in, line, '\n')) {
		if (line.empty()) {
			continue;
		}
		std::smatch result;
		if (std::regex_match(line, result, foldLine)) {
			int foldValue = std::stoi(result.str(2));
			std::unordered_set<std::pair<int, int>, PairHasher> newPoints;
			if (result.str(1) == "x") {
				for (auto& [x, y] : points) {
					std::pair<int, int> newPoint(x, y);
					if (x > foldValue) {
						newPoint.first = 2 * foldValue - x;
					}
					newPoints.insert(newPoint);
				}
			} else {
				for (auto& [x, y] : points) {
					std::pair<int, int> newPoint(x, y);
					if (y > foldValue) {
						newPoint.second = 2 * foldValue - y;
					}
					newPoints.insert(newPoint);
				}
			}
			points.swap(newPoints);
			continue;
		}
		size_t split = line.find(',');
		int x = std::stoi(line.substr(0, split));
		int y = std::stoi(line.substr(split + 1));
		points.emplace(x, y);
	}

	int maxX = -1, maxY = -1;
	for (const std::pair<int, int>& point : points) {
		maxX = std::max(maxX, point.first);
		maxY = std::max(maxY, point.second);
	}
	std::vector<std::vector<char>> letters(maxY + 1);
	for (std::vector<char>& rows : letters) {
		rows.resize(maxX + 1, ' ');
	}
	for (const std::pair<int, int>& point : points) {
		letters[point.second][point.first] = '*';
	}
	for (const std::vector<char>& row : letters) {
		for (char c : row) {
			std::cout << c;
		}
		std::cout << std::endl;
	}

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
}
