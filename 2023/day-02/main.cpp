#include <string>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "input_selector.h"

static int getGameNum(const std::string& gameId) {
	std::vector<std::string> gameSplit;
	boost::split(gameSplit, gameId, boost::is_any_of(" "));
	return std::stoi(gameSplit[1]);
}

static std::pair<int, std::string> getCubes(const std::string& draw) {
	std::vector<std::string> drawSplit;
	boost::split(drawSplit, draw, boost::is_any_of(" "));
	return {std::stoi(drawSplit[0]), drawSplit[1]};
}

static bool isPossibleGame(const std::string& draw) {
	auto [cubeNum, color] = getCubes(draw);
	if (color == "red") {
		return cubeNum <= 12;
	} else if (color == "green") {
		return cubeNum <= 13;
	} else if (color == "blue") {
		return cubeNum <= 14;
	}
	return true;
}

int f1(std::istream& in) {
	std::string line;
	int sumOfPossibleGames = 0;
	while (std::getline(in, line)) {
		std::vector<std::string> tokens;
		boost::split(tokens, line, boost::algorithm::is_any_of(":"));
		boost::trim(tokens[1]);

		std::vector<std::string> draws;
		boost::split(draws, tokens[1], boost::algorithm::is_any_of(",;"));
		bool isPossible = true;
		for (std::string& draw : draws) {
			boost::trim(draw);
			isPossible = isPossible && isPossibleGame(draw);
		}
		if (isPossible) {
			sumOfPossibleGames += getGameNum(tokens[0]);
		}
	}
	std::cout << sumOfPossibleGames << '\n';
	return 0;
}

int f2(std::istream& in) {
	int sumOfPowers = 0;
	std::string line;
	while (std::getline(in, line)) {
		std::vector<std::string> tokens;
		boost::split(tokens, line, boost::algorithm::is_any_of(":"));
		boost::trim(tokens[1]);

		std::vector<std::string> draws;
		boost::split(draws, tokens[1], boost::algorithm::is_any_of(",;"));

		std::unordered_map<std::string, int> colorCounts{{"red", 0}, {"green", 0}, {"blue", 0}};
		for (std::string& draw : draws) {
			boost::trim(draw);
			auto [num, color] = getCubes(draw);
			if (colorCounts[color] < num) {
				colorCounts[color] = num;
			}
		}
		int power = 1;
		for (const auto& [color, num] : colorCounts) {
			power *= num;
		}
		sumOfPowers += power;
	}
	std::cout << sumOfPowers << '\n';
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}