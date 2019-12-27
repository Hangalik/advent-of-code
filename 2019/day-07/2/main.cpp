#include <iostream>
#include <algorithm>
#include <deque>
#include "steppingIntCode.h"

int main() {

	std::deque<int> icIn;
	std::deque<int> icOut;
	const SteppingIntCode ic{ std::cin, icIn, icOut };

	std::vector<int> phaseCodes{ 5, 6, 7, 8, 9 };
	int maxAmplification{ 0 };
	do {
		std::deque<int> aAmpInput{ phaseCodes[0], 0 };
		std::deque<int> bAmpInput{ phaseCodes[1] };
		std::deque<int> cAmpInput{ phaseCodes[2] };
		std::deque<int> dAmpInput{ phaseCodes[3] };
		std::deque<int> eAmpInput{ phaseCodes[4] };
		std::vector<SteppingIntCode> amps {
			SteppingIntCode{ ic, aAmpInput, bAmpInput },
			SteppingIntCode{ ic, bAmpInput, cAmpInput },
			SteppingIntCode{ ic, cAmpInput, dAmpInput },
			SteppingIntCode{ ic, dAmpInput, eAmpInput },
			SteppingIntCode{ ic, eAmpInput, aAmpInput }
		};

		ComputationProgress progress{ ComputationProgress::WAITING_FOR_INPUT };
		while (progress != ComputationProgress::TERMINATED) {
			for (auto& amp : amps) {
				progress = amp.step();
				if (progress == ComputationProgress::ERROR) {
					return 1;
				}
			}
		}

		if (aAmpInput.size() != 1) {
			std::cerr << "Error: Result has other than 1 results!" << std::endl;
			return 1;
		}

		if (aAmpInput.front() > maxAmplification) {
			maxAmplification = aAmpInput.front();
		}
	} while (std::next_permutation(phaseCodes.begin(), phaseCodes.end()));

	std::cout << "The result is: " << maxAmplification << std::endl;

	return 0;
}

