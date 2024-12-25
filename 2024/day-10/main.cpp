#include <iostream>
#include <vector>
#include <unordered_set>
#include <ranges>

#include "input_selector.h"

struct PairHasher {
	static size_t operator()(const std::pair<size_t, size_t>& v) {
		return 31 * std::hash<size_t>{}(v.first) + std::hash<size_t>{}(v.second);
	}
};

int walkTrailhead(const std::vector<std::string>& map, size_t row, size_t column, std::unordered_set<std::pair<size_t, size_t>, PairHasher>& peaks) {
	if (map[row][column] == '9') {
		peaks.emplace(row, column);
		return 1;
	}
	int rating = 0;
	char search = map[row][column] + 1;
	if (row > 0 && map[row - 1][column] == search) {
		rating += walkTrailhead(map, row - 1, column, peaks);
	}
	if (column > 0 && map[row][column - 1] == search) {
		rating += walkTrailhead(map, row, column - 1, peaks);
	}
	if (row < map.size() - 1 && map[row + 1][column] == search) {
		rating += walkTrailhead(map, row + 1, column, peaks);
	}
	if (column < map[row].size() - 1 && map[row][column + 1] == search) {
		rating += walkTrailhead(map, row, column + 1, peaks);
	}
	return rating;
}

size_t getScoreOfMap(const std::vector<std::string>& map) {
	size_t score = 0;
	for (size_t i = 0uz; i < map.size(); i++) {
		for (size_t j = 0uz; j < map[i].size(); j++) {
			if (map[i][j] == '0') {
				std::unordered_set<std::pair<size_t, size_t>, PairHasher> peaks;
				walkTrailhead(map, i, j, peaks);
				score += peaks.size();
			}
		}
	}
	return score;
}

int f1(std::istream& in) {
	std::vector<std::string> map;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		map.push_back(line);
	}

	size_t score = getScoreOfMap(map);
	std::cout << score << "\n";

	return 0;
}

int getRatingOfMap(const std::vector<std::string>& map) {
	int rating = 0;
	std::unordered_set<std::pair<size_t, size_t>, PairHasher> peaks;
	for (size_t i = 0uz; i < map.size(); i++) {
		for (size_t j = 0uz; j < map[i].size(); j++) {
			if (map[i][j] == '0') {
				rating += walkTrailhead(map, i, j, peaks);
			}
		}
	}
	return rating;
}

int f2(std::istream& in) {
	std::vector<std::string> map;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) continue;
		map.push_back(line);
	}

	int rating = getRatingOfMap(map);
	std::cout << rating << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}