#include <iostream>

#include "input_selector.h"

void printFish(const std::vector<int>& fish) {
	for (size_t i = 0; i < fish.size(); i++) {
		std::cout << fish[i] << ",";
	}
	std::cout << std::endl;
}

int f1(std::istream& in) {
	std::vector<int> fish;
	int num;
	while (in >> num) {
		in.ignore();
		fish.push_back(num);
	}
	for (int i = 0; i < 80; i++) {
		size_t population = fish.size();
		for (size_t j = 0; j < population; j++) {
			if (fish[j] == 0) {
				fish[j] = 6;
				fish.push_back(8);
			} else {
				fish[j]--;
			}
		}
	}

	std::cout << fish.size() << std::endl;

	return 0;
}

int f2(std::istream& in) {
	unsigned long long fish[9] = { 0 };
	int num;
	while (in >> num) {
		in.ignore();
		fish[num]++;
	}
	for (int i = 0; i < 256; i++) {
		unsigned long long newFish[9] = { 0 };
		newFish[6] += fish[0];
		newFish[8] += fish[0];
		for (int j = 1; j < 9; j++) {
			newFish[j - 1] += fish[j];
		}
		std::swap(fish, newFish);
	}

	unsigned long long sum = 0;
	for (int i = 0; i < 9; i++) {
		sum += fish[i];
	}

	std::cout << sum << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);
}