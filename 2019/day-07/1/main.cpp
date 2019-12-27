#include <iostream>
#include <algorithm>
#include "simpleIntCode.h"

int main() {

	const SimpleIntCode ic{ std::cin };

	std::vector<int> phaseCodes{ 0, 1, 2, 3, 4 };
	int maxAmplification{ 0 };
	do {
		int resultOfPrevious{ 0 };
		for (int phaseCode : phaseCodes) {
			std::vector<int> input{ phaseCode, resultOfPrevious };
			std::vector<int> output;
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

