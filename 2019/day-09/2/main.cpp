#include <iostream>
#include "simpleIntCode.h"

int main() {

	using value_type = typename SimpleIntCode::value_type;
	SimpleIntCode sic{ std::cin };
	std::vector<value_type> input{ 2 };
	std::vector<value_type> output;
	sic.compute(input, output);

	for (long long i : output) {
		std::cout << i << std::endl;
	}

	return 0;
}
