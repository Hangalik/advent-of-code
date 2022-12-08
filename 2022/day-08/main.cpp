#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

#include "input_selector.h"

std::vector<std::vector<int>> readTrees(std::istream& in) {
	std::vector<std::vector<int>> trees;
	std::string line;
	while (std::getline(in, line)) {
		std::vector<int> treeRow;
		std::transform(line.begin(), line.end(),
						std::back_inserter(treeRow),
						[](char c) { return int(c - '0'); });
		trees.push_back(std::move(treeRow));
	}
	return trees;
}

bool isVisible(const std::vector<std::vector<int>>& trees, size_t x, size_t y) {
	int height = trees[x][y];
	auto rowPredicate = std::bind(std::less<int>(), std::placeholders::_1, height);

	bool visibleFromLeft = std::all_of(trees[x].begin(), trees[x].begin() + y, rowPredicate);
	if (visibleFromLeft) {
		return true;
	}

	bool visibleFromRight = std::all_of(trees[x].begin() + y + 1, trees[x].end(), rowPredicate);
	if (visibleFromRight) {
		return true;
	}

	auto columnPredicate = [y, height](const std::vector<int>& row) { return row[y] < height; };

	bool visibleFromAbove = std::all_of(trees.begin(), trees.begin() + x, columnPredicate);
	if (visibleFromAbove) {
		return true;
	}

	bool visibleFromBelow = std::all_of(trees.begin() + x + 1, trees.end(), columnPredicate);
	return visibleFromBelow;
}

int f1(std::istream& in) {

	std::vector<std::vector<int>> trees = readTrees(in);

	int visibleTrees = 0;
	for (size_t i = 0; i < trees.size(); i++) {
		for (size_t j = 0; j < trees[i].size(); j++) {
			if (isVisible(trees, i, j)) {
				visibleTrees++;
			}
		}
	}

	std::cout << visibleTrees << "\n";

	return 0;
}

int calculateDirectionScore(auto start, auto end, auto predicate) {
	int directionScore = 0;
	while (start != end) {
		directionScore++;
		if (!predicate(*start)) {
			break;
		}
		start++;
	}
	return directionScore;
}

int calculateScenicScore(const std::vector<std::vector<int>>& trees, size_t x, size_t y) {
	int height = trees[x][y];
	auto rowPredicate = std::bind(std::less<int>(), std::placeholders::_1, height);

	int rightScore = calculateDirectionScore(trees[x].begin() + y + 1, trees[x].end(), rowPredicate);
	int leftScore = calculateDirectionScore(trees[x].rbegin() + (trees[x].size() - y), trees[x].rend(), rowPredicate);

	auto columnPredicate = [y, height](const std::vector<int>& row) { return row[y] < height; };

	int bottomScore = calculateDirectionScore(trees.begin() + x + 1, trees.end(), columnPredicate);
	int topScore = calculateDirectionScore(trees.rbegin() + (trees.size() - x), trees.rend(), columnPredicate);

	return rightScore * leftScore * bottomScore * topScore;
}

int f2(std::istream& in) {

	std::vector<std::vector<int>> trees = readTrees(in);

	int maxScenicScore = 0;
	for (size_t i = 0; i < trees.size(); i++) {
		for (size_t j = 0; j < trees[i].size(); j++) {
			int scenicScore = calculateScenicScore(trees, i, j);
			maxScenicScore = std::max(maxScenicScore, scenicScore);
		}
	}

	std::cout << maxScenicScore << "\n";

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