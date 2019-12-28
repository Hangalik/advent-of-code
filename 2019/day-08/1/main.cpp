#include <iostream>
#include <limits>

constexpr int width = 25;
constexpr int height = 6;

int main() {

	int fewestZeros{ std::numeric_limits<int>::max() };
	int onesTwosMultiplied{ -1 };
	while (true) {
		int zeroCount{ 0 };
		int oneCount{ 0 };
		int twoCount{ 0 };
		for (int i = 0; i < width * height; i++) {
			char c;
			if (!std::cin.get(c)) {
				std::cout << "The count of ones multiplied by the count of twos is " << onesTwosMultiplied << std::endl;
				return 0;
			}
			switch (c) {
			case '0':
				zeroCount++;
				break;
			case '1':
				oneCount++;
				break;
			case '2':
				twoCount++;
				break;
			default:
				break;
			}
		}
		if (zeroCount < fewestZeros) {
			fewestZeros = zeroCount;
			onesTwosMultiplied = oneCount * twoCount;
		}
	}

	return 0;
}
