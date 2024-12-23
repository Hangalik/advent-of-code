#include <iostream>
#include <vector>
#include <algorithm>

#include "input_selector.h"

// Test cases
// static constexpr int width = 11;
// static constexpr int height = 7;

// Real
static constexpr int width = 101;
static constexpr int height = 103;

static constexpr int widthThreshold = width / 2;
static constexpr int heightThreshold = height / 2;

struct Robot {
	int px, py, vx, vy;
};

std::vector<Robot> readRobots(std::istream& in) {
	std::vector<Robot> robots;
	std::string line;
	while (std::getline(in, line)) {
		size_t pxStart = 2;
		size_t pxEnd = line.find(',');
		size_t pyStart = pxEnd + 1;
		size_t pyEnd = line.find(' ');
		size_t vxStart = pyEnd + 3;
		size_t vxEnd = line.find(',', vxStart);
		size_t vyStart = vxEnd + 1;

		Robot robot;
		robot.px = std::stoi(line.substr(pxStart, pxEnd - pxStart));
		robot.py = std::stoi(line.substr(pyStart, pyEnd - pyStart));
		robot.vx = std::stoi(line.substr(vxStart, vxEnd - vxStart));
		robot.vy = std::stoi(line.substr(vyStart));
		robots.push_back(robot);
	}
	return robots;
}

int f1(std::istream& in) {
	std::vector<Robot> robots = readRobots(in);

	int q1Count = 0, q2Count = 0, q3Count = 0, q4Count = 0;
	for (const Robot& r : robots) {
		int px = (r.px + 100 * r.vx) % width;
		int py = (r.py + 100 * r.vy) % height;
		if (px < 0) px += width;
		if (py < 0) py += height;
		if (px < widthThreshold && py < heightThreshold) q1Count++;
		if (px > widthThreshold && py < heightThreshold) q2Count++;
		if (px < widthThreshold && py > heightThreshold) q3Count++;
		if (px > widthThreshold && py > heightThreshold) q4Count++;
	}

	std::cout << q1Count * q2Count * q3Count * q4Count << "\n";

	return 0;
}

struct RobotComparator {
	static bool operator()(const Robot& a, const Robot& b) {
		if (a.py != b.py) return a.py < b.py;
		return a.px < b.px;
	}
};

void print(std::vector<Robot>& robots) {
	std::ranges::sort(robots, RobotComparator{});
	auto it = robots.begin();
	std::string line(height * (width + 1), '.');
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bool robot = std::ranges::binary_search(robots, Robot{i, j, 0, 0}, RobotComparator{});
			if (robot) {
				int idx = i * (width + 1) + j;
				line[idx] = '#';
			}
		}
		int idx = i * (width + 1) + width;
		line[idx] = '\n';
	}
	std::cout << line;
}

int f2(std::istream& in) {
	std::vector<Robot> robots = readRobots(in);

	print(robots);

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}