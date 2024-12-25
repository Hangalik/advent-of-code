#include <iostream>

#include "input_selector.h"

std::pair<long long, long long> getVector(const std::string& line) {
	size_t xStart = line.find('X') + 2;
	size_t xEnd = line.find(',');
	long long x = std::stoll(line.substr(xStart, xEnd - xStart));
	size_t yStart = line.find('Y') + 2;
	long long y = std::stoll(line.substr(yStart));
	return {x, y};
}

bool isPossible(long long count, long long det) {
	if (count % det != 0) {
		return false;
	}
	return !((count < 0) ^ (det < 0));
}

/**
 * Matrixes: X*A=B, where X is unknown.
 * X=B*A^-1, where A^-1 is the inverse of A.
 * If A = ((a1, a2), (a3, a4)), then A^-1 = ((a4, -a2), (-a3, a1)) / det, where det is the determinant of A.
 * Button A is the first row of A, button B is the second row.
 * Prize is B, and we are looking for X.
 */
void solve(std::istream& in, long long offset) {
	long long tokens = 0;
	std::string buttonALine;
	std::string buttonBLine;
	std::string prizeLine;
	while (std::getline(in, buttonALine)
		&& std::getline(in, buttonBLine)
		&& std::getline(in, prizeLine))
	{
		auto [b1x, b1y] = getVector(buttonALine);
		auto [b2x, b2y] = getVector(buttonBLine);
		auto [cx, cy]   = getVector(prizeLine);
		cx += offset;
		cy += offset;
		long long det = b1x*b2y - b1y*b2x;

		long long aCount = b2y*cx - b2x*cy;
		long long bCount = -b1y*cx + b1x*cy;

		if (isPossible(aCount, det) && isPossible(bCount, det)) {
			tokens += 3 * aCount / det + bCount / det;
		}

		std::string emptyLine;
		std::getline(in, emptyLine);
	}

	std::cout << tokens << "\n";
}

int f1(std::istream& in) {
	solve(in, 0);
	return 0;
}

int f2(std::istream& in) {
	solve(in, 10'000'000'000'000);
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}