#include <iostream>
#include "simpleIntCode.h"

int main() {

	SimpleIntCode sic{ std::cin };
	std::vector<long long> input{ 1 };
	std::vector<long long> output;
	sic.compute(input, output);

	for (long long i : output) {
		std::cout << i << std::endl;
	}

	return 0;
}
