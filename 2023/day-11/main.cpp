#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <ranges>
#include <tuple>

#include "input_selector.h"

void expand(std::vector<std::pair<size_t, size_t>>& galaxies, auto projection, size_t expansionRate) {
	std::ranges::sort(galaxies, {}, projection);
	size_t last = std::invoke(projection, galaxies[0]);
	size_t extraSpace = 0;
	for (size_t& galaxy : galaxies | std::views::transform(projection)) {
		size_t delta = galaxy - last;
		last = galaxy;
		if (delta > 1) {
			extraSpace += (delta - 1) * (expansionRate - 1);
		}
		galaxy += extraSpace;
	}
}

void printGalaxies(const std::vector<std::pair<size_t, size_t>>& galaxies) {
	for (const auto& galaxy : galaxies) {
		std::cout << "(" << galaxy.first << ", " << galaxy.second << ")\n";
	}
	std::cout << "---\n";
}

void ff(std::istream& in, size_t expansionRate) {
	std::vector<std::pair<size_t, size_t>> galaxies;
	std::string line;
	size_t lineIndex = 0;
	while (std::getline(in, line)) {
		size_t searchPos = -1;
		while ((searchPos = line.find('#', searchPos + 1)) != std::string::npos) {
			galaxies.emplace_back(lineIndex, searchPos);
		}
		lineIndex++;
	}

	expand(galaxies, &std::pair<size_t, size_t>::first, expansionRate);
	expand(galaxies, &std::pair<size_t, size_t>::second, expansionRate);

	long long sum = 0;
	auto diff = [](size_t a, size_t b) {
		if (a > b) return a - b;
		else return b - a;
	};
	for (auto [a, b] : std::views::cartesian_product(galaxies, galaxies)) {
		sum += diff(a.first, b.first) + diff(a.second, b.second);
	}

	std::cout << sum / 2 << "\n";
}

int f1(std::istream& in) {
	ff(in, 2);
	return 0;
}

int f2(std::istream& in) {
	ff(in, 1'000'000);
	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}