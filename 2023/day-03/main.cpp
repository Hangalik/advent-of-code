#include <string>
#include <vector>
#include <cctype>
#include <print>
#include <ranges>
#include <charconv>

#include "input_selector.h"

bool isSymbol(char c) {
	return c != '.' && !std::isdigit(c);
}

bool isAdjacentSymbol(const std::vector<std::string>& lines, size_t i, size_t j) {
	size_t il = i == 0 ? 0 : i - 1;
	size_t ih = i == lines.size() - 1 ? lines.size() - 1 : i + 1;
	size_t jl = j == 0 ? 0 : j - 1;
	size_t jh = j == lines[i].size() - 1 ? lines[i].size() - 1 : j + 1;

	for (size_t ii = il; ii <= ih; ii++) {
		for (size_t jj = jl; jj <= jh; jj++) {
			if (isSymbol(lines[ii][jj])) {
				return true;
			}
		}
	}
	return false;
}

int f1(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(in, line)) {
		lines.push_back(std::move(line));
	}

	int sum = 0;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < lines[i].size(); j++) {
			if (!std::isdigit(lines[i][j])) {
				continue;
			}
			bool symbolAdjacent = false;
			int num = 0;
			while (j < lines[i].size() && std::isdigit(lines[i][j])) {
				if (isAdjacentSymbol(lines, i, j)) {
					symbolAdjacent = true;
				}
				num *= 10;
				num += (int) (lines[i][j] - '0');
				j++;
			}
			if (symbolAdjacent) {
				sum += num;
			}
		}
	}

	std::println("{}", sum);

	return 0;
}

int parseNumberAt(const std::string& line, size_t j) {
	size_t startOfNumber = j;
	while (startOfNumber > 0 && std::isdigit(line[startOfNumber - 1])) {
		startOfNumber--;
	}
	size_t endOfNumber = j;
	while (endOfNumber < line.size() - 1 && std::isdigit(line[endOfNumber + 1])) {
		endOfNumber++;
	}
	int number;
	std::from_chars(line.data() + startOfNumber, line.data() + endOfNumber + 1, number);
	return number;
}

int countAdjecentNumbers(const std::vector<std::string>& lines, size_t i, size_t j) {
	size_t maxIndex = lines[i].size() - 1;
	int count = 0;
	int product = 1;
	if (j > 0 && std::isdigit(lines[i][j - 1])) {
		count++;
		product *= parseNumberAt(lines[i], j - 1);
	}
	if (j < maxIndex && std::isdigit(lines[i][j + 1])) {
		count++;
		product *= parseNumberAt(lines[i], j + 1);
	}

	if (i > 0) {
		const std::string& topLine = lines[i - 1];
		bool topleft = (j > 0 && std::isdigit(topLine[j - 1]));
		bool topmiddle = (std::isdigit(topLine[j]));
		bool topright = (j < maxIndex && std::isdigit(topLine[j + 1]));
		if (topleft && !topmiddle && topright) {
			count += 2;
			product *= parseNumberAt(topLine, j - 1);
			product *= parseNumberAt(topLine, j + 1);
		} else if (topleft || topmiddle || topright) {
			count++;
			if (topleft) product *= parseNumberAt(topLine, j - 1);
			else if (topmiddle) product *= parseNumberAt(topLine, j);
			else if (topright) product *= parseNumberAt(topLine, j + 1);
		}
	}

	if (i < lines.size() - 1) {
		const std::string& bottomLine = lines[i + 1];
		bool bottomleft = (j > 0 && std::isdigit(bottomLine[j - 1]));
		bool bottommiddle = (std::isdigit(bottomLine[j]));
		bool bottomright = (j < maxIndex && std::isdigit(bottomLine[j + 1]));
		if (bottomleft && !bottommiddle && bottomright) {
			count += 2;
			product *= parseNumberAt(bottomLine, j - 1);
			product *= parseNumberAt(bottomLine, j + 1);
		} else if (bottomleft || bottommiddle || bottomright) {
			count++;
			if (bottomleft) product *= parseNumberAt(bottomLine, j - 1);
			else if (bottommiddle) product *= parseNumberAt(bottomLine, j);
			else if (bottomright) product *= parseNumberAt(bottomLine, j + 1);
		}
	}
	if (count != 2) {
		return 0;
	}
	return product;
}

int f2(std::istream& in) {
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(in, line)) {
		lines.push_back(std::move(line));
	}

	int sum = 0;
	for (auto [i, j] : std::views::cartesian_product(std::views::iota(0uz, lines.size()), std::views::iota(0uz, lines[0].size()))) {
		char c = lines[i][j];
		if (c != '*') {
			continue;
		}
		sum += countAdjecentNumbers(lines, i, j);
	}

	std::println("{}", sum);

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}