#include <iostream>
#include "simpleIntCode.h"

int main() {

	using value_type = SimpleIntCode::value_type;
	SimpleIntCode sic{ std::cin };
	std::vector<value_type> input{ 1 };
	std::vector<value_type> output;
	sic.compute(input, output);

	for (long long i : output) {
		std::cout << i << std::endl;
	}

	return 0;
}
