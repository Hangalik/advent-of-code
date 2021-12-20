#include <iostream>
#include <string>
#include <regex>
#include <utility>
#include <cmath>

#include "input_selector.h"

std::pair<int, int> getValuePair(const std::string& str) {
	size_t firstStart = 0;
	size_t firstEnd = str.find("..");
	size_t secondStart = firstEnd + 2;
	size_t secondEnd = std::string::npos;
	int first = std::stoi(str.substr(firstStart, firstEnd));
	int second = std::stoi(str.substr(secondStart, secondEnd));
	return {first, second};
}

int f1(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');

	size_t xStart = line.find("x=") + 2;
	size_t xEnd = line.find(", ", xStart);
	std::string xStr = line.substr(xStart, xEnd - xStart);
	//auto [xLow, xHigh] = getValuePair(xStr);

	size_t yStart = line.find("y=") + 2;
	std::string yStr = line.substr(yStart);
	auto [yLow, yHigh] = getValuePair(yStr);

	int maxYSpeed = std::abs(yLow) - 1;
	int maxHeight = (maxYSpeed * (maxYSpeed + 1)) / 2;

	std::cout << maxHeight << std::endl;

	return 0;
}

int calculateMinimumXSpeed(int xLow) {
	int minXSpeed = 0;
	int totalX = 0;
	while (totalX < xLow) {
		minXSpeed++;
		totalX += minXSpeed;
	}
	return minXSpeed;
}

struct Area {
	int xLow;
	int xHigh;
	int yLow;
	int yHigh;
};

struct Speed {
	int x;
	int y;
};

int sgn(int x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

int decrementXSpeed(int xSpeed) {
	return xSpeed - sgn(xSpeed);
}

bool willItHit(Speed speed, const Area& target) {
	int xPos = 0, yPos = 0;
	while (true) {
		xPos += speed.x;
		yPos += speed.y;
		speed.x = decrementXSpeed(speed.x);
		speed.y--;
		if (xPos >= target.xLow && xPos <= target.xHigh &&
				yPos >= target.yLow && yPos <= target.yHigh) {
			return true;
		}
		if (xPos > target.xHigh || yPos < target.yLow) {
			break;
		}
	}
	return false;
}

int f2(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');

	size_t xStart = line.find("x=") + 2;
	size_t xEnd = line.find(", ", xStart);
	std::string xStr = line.substr(xStart, xEnd - xStart);
	auto [xLow, xHigh] = getValuePair(xStr);

	size_t yStart = line.find("y=") + 2;
	std::string yStr = line.substr(yStart);
	auto [yLow, yHigh] = getValuePair(yStr);

	int maxYSpeed = std::abs(yLow) - 1;
	int minYSpeed = yLow - 1;
	int maxXSpeed = xHigh + 1;
	int minXSpeed = calculateMinimumXSpeed(xLow);

	Area target{xLow, xHigh, yLow, yHigh};

	int numberOfHittingSpeeds = 0;
	for (int xSpeed = minXSpeed; xSpeed <= maxXSpeed; xSpeed++) {
		for (int ySpeed = minYSpeed; ySpeed <= maxYSpeed; ySpeed++) {
			Speed speed{xSpeed, ySpeed};
			if (willItHit(speed, target)) {
				numberOfHittingSpeeds++;
			}
		}
	}

	std::cout << numberOfHittingSpeeds << std::endl;

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
