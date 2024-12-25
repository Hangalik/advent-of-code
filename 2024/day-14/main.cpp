#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

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

void passTime(std::vector<Robot>& robots, std::string& line, int seconds) {
	for (int i = 0; i < height; i++) {
		size_t lineStart = i * (width + 1);
		auto lineBegin = line.begin() + lineStart;
		auto lineEnd = lineBegin + width;
		std::fill(lineBegin, lineEnd, '.');
		*lineEnd = '\n';
	}
	for (Robot& r : robots) {
		r.px = (r.px + seconds * r.vx) % width;
		r.py = (r.py + seconds * r.vy) % height;
		if (r.px < 0) r.px += width;
		if (r.py < 0) r.py += height;
		int idx = r.py * (width + 1) + r.px;
		line[idx] = '#';
	}
}

int f2(std::istream& in) {
	std::vector<Robot> robots = readRobots(in);
	std::string robotStr(height * (width + 1), '.');
	passTime(robots, robotStr, 0);

	int idx = 0;
	std::cout << idx << ":\n" << robotStr << "\n";

	int timeToPass = 7055;
	std::string line("");
	while (std::getline(std::cin, line)) {
		if (line == "q") break;
		try {
			timeToPass = std::stoi(line);
			if (timeToPass <= 0) timeToPass = 1;
		} catch (...) {
			;
		}
		idx += timeToPass;
		passTime(robots, robotStr, timeToPass);
		std::cout << idx << ":\n" << robotStr << "\n";
	}

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}