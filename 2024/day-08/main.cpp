#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>
#include <ranges>

#include "input_selector.h"

std::pair<int, int> minus(const std::pair<int, int>& a, const std::pair<int, int>& b) {
	return std::make_pair(a.first - b.first, a.second - b.second);
}

std::pair<int, int> plus(const std::pair<int, int>& a, const std::pair<int, int>& b) {
	return std::make_pair(a.first + b.first, a.second + b.second);
}

int f1(std::istream& in) {
	std::unordered_map<char, std::vector<std::pair<int, int>>> towers;
	int row = 0;
	int maxColumn = 0;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		maxColumn = line.size();
		for (size_t column = 0uz; column < line.size(); column++) {
			if (std::isalnum(line[column])) {
				towers[line[column]].push_back(std::make_pair(row, (int)column));
			}
		}
		row++;
	}
	int maxRow = row;

	// unordered_set would be better, but then I'd need to write my own hash implementation, because std::pair doesn't have one
	std::set<std::pair<int, int>> antinodes;
	for (const std::vector<std::pair<int, int>>& towerGroup : std::views::values(towers)) {
		for (size_t i = 0; i < towerGroup.size() - 1; i++) {
			for (size_t j = i + 1; j < towerGroup.size(); j++) {
				const std::pair<int, int>& tower1 = towerGroup[i];
				const std::pair<int, int>& tower2 = towerGroup[j];
				std::pair<int, int> antinode1 = plus(tower1, minus(tower1, tower2));
				std::pair<int, int> antinode2 = plus(tower2, minus(tower2, tower1));
				if (antinode1.first >= 0 && antinode1.first < maxRow && antinode1.second >= 0 && antinode1.second < maxColumn) {
					antinodes.insert(antinode1);
				}
				if (antinode2.first >= 0 && antinode2.first < maxRow && antinode2.second >= 0 && antinode2.second < maxColumn) {
					antinodes.insert(antinode2);
				}
			}
		}
	}

	std::cout << antinodes.size() << "\n";

	return 0;
}

std::pair<int, int> times(const std::pair<int, int>& a, int b) {
	return std::make_pair(a.first * b, a.second * b);
}

int f2(std::istream& in) {
	std::unordered_map<char, std::vector<std::pair<int, int>>> towers;
	int row = 0;
	int maxColumn = 0;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		maxColumn = line.size();
		for (size_t column = 0uz; column < line.size(); column++) {
			if (std::isalnum(line[column])) {
				towers[line[column]].push_back(std::make_pair(row, (int)column));
			}
		}
		row++;
	}
	int maxRow = row;

	// unordered_set would be better, but then I'd need to write my own hash implementation, because std::pair doesn't have one
	std::set<std::pair<int, int>> antinodes;
	for (const std::vector<std::pair<int, int>>& towerGroup : std::views::values(towers)) {
		for (size_t i = 0; i < towerGroup.size() - 1; i++) {
			for (size_t j = i + 1; j < towerGroup.size(); j++) {
				const std::pair<int, int>& tower1 = towerGroup[i];
				const std::pair<int, int>& tower2 = towerGroup[j];
				int idx = 0;
				std::pair<int, int> step21 = minus(tower1, tower2);
				while (true) {
					std::pair<int, int> antinode = plus(tower1, times(step21, idx));
					if (antinode.first < 0 || antinode.first >= maxRow || antinode.second < 0 || antinode.second >= maxColumn) {
						break;
					}
					antinodes.insert(antinode);
					idx++;
				}
				idx = 0;
				std::pair<int, int> step12 = minus(tower2, tower1);
				while (true) {
					std::pair<int, int> antinode = plus(tower2, times(step12, idx));
					if (antinode.first < 0 || antinode.first >= maxRow || antinode.second < 0 || antinode.second >= maxColumn) {
						break;
					}
					antinodes.insert(antinode);
					idx++;
				}
			}
		}
	}

	std::cout << antinodes.size() << "\n";
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}