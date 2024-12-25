#include <iostream>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <algorithm>
#include <functional>

#include "input_selector.h"

using stone_t = unsigned long long;

std::unordered_map<stone_t, long long> blink(const std::unordered_map<stone_t, long long>& stones) {
	std::unordered_map<stone_t, long long> newStones;
	for (auto [stone, count] : stones) {
		if (stone == 0) {
			newStones[1] += count;
		} else {
			std::string stoneStr = std::to_string(stone);
			if (stoneStr.size() % 2 == 0) {
				size_t half = stoneStr.size() / 2;
				stone_t first = std::stoull(stoneStr.substr(0, half));
				stone_t second = std::stoull(stoneStr.substr(half));
				newStones[first] += count;
				newStones[second] += count;
			} else {
				newStones[stone * 2024] += count;
			}
		}
	}
	return newStones;
}

void solve(std::istream& in, int iterations) {
	std::unordered_map<stone_t, long long> stones;
	stone_t stone;
	while (in >> stone) {
		stones[stone]++;
	}

	for (int i = 0; i < iterations; i++) {
		stones = blink(stones);
	}

	long long count = std::ranges::fold_left(std::views::values(stones), 0LL, std::plus<>{});
	std::cout << count << "\n";
}

int f1(std::istream& in) {

	solve(in, 25);

	return 0;
}

int f2(std::istream& in) {

	solve(in, 75);

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}