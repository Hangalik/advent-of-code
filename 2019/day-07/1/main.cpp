#include <iostream>
#include <algorithm>
#include "simpleIntCode.h"

int main() {

	using value_type = typename SimpleIntCode::value_type;
	const SimpleIntCode ic{ std::cin };

	std::vector<int> phaseCodes{ 0, 1, 2, 3, 4 };
	int maxAmplification{ 0 };
	do {
		int resultOfPrevious{ 0 };
		for (int phaseCode : phaseCodes) {
			std::vector<value_type> input{ phaseCode, resultOfPrevious };
			std::vector<value_type> output;
			SimpleIntCode amp{ ic };
			amp.compute(input, output);
			resultOfPrevious = output[0];
		}
		if (resultOfPrevious > maxAmplification) {
			maxAmplification = resultOfPrevious;
		}
	} while (std::next_permutation(phaseCodes.begin(), phaseCodes.end()));

	std::cout << "The result is: " << maxAmplification << std::endl;

	return 0;
}

