#include <iostream>
#include <sstream>
#include <vector>
#include <ranges>
#include <algorithm>

#include "input_selector.h"

std::vector<unsigned> getNumbers(std::string in) {
	std::vector<unsigned> numbers;
	std::stringstream inStream(std::move(in));
	unsigned num;
	while (inStream >> num) {
		numbers.push_back(num);
	}
	return numbers;
}

template <typename T>
T countWinningStrategies(const std::tuple<T, T>& timesAndDistances) {
	T time = std::get<0>(timesAndDistances);
	T distance = std::get<1>(timesAndDistances);
	T count = 0;
	for (unsigned i = 0; i <= time; i++) {
		if (i * (time - i) > distance) {
			count++;
		}
	}
	return count;
}

int f1(std::istream& in) {
	std::string timeLine;
	std::getline(in, timeLine);
	std::string distanceLine;
	std::getline(in, distanceLine);

	timeLine = timeLine.substr(10);
	distanceLine = distanceLine.substr(10);

	std::vector<unsigned> times = getNumbers(timeLine);
	std::vector<unsigned> distances = getNumbers(distanceLine);

	unsigned result = *std::ranges::fold_left_first(
		std::views::zip(times, distances) | std::views::transform(countWinningStrategies<unsigned>),
		std::multiplies<>()
	);

	std::cout << result << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string timeLine;
	std::getline(in, timeLine);
	std::string distanceLine;
	std::getline(in, distanceLine);

	timeLine = timeLine.substr(10);
	distanceLine = distanceLine.substr(10);

	auto timeRet = std::ranges::remove(timeLine, ' ');
	timeLine.erase(timeRet.begin(), timeRet.end());
	auto distanceRet = std::ranges::remove(distanceLine, ' ');
	distanceLine.erase(distanceRet.begin(), distanceRet.end());

	unsigned long long time = std::stoull(timeLine);
	unsigned long long distance = std::stoull(distanceLine);

	unsigned long long result = countWinningStrategies(std::make_tuple(time, distance));

	std::cout << result << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}