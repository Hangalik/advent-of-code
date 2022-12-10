#include <iostream>
#include <unordered_set>
#include <array>
#include <algorithm>

#include "input_selector.h"

int interestingSignalStrenths(int regX, int cycleNumber) {
	static const std::unordered_set<int> interestingCycles{20, 60, 100, 140, 180, 220};
	if (interestingCycles.contains(cycleNumber)) {
		return regX * cycleNumber;
	}
	return 0;
}

int f1(std::istream& in) {
	int regX = 1;
	int cycleNumber = 0;
	int sumOfInterestingSignalStrengths = 0;
	std::string command;
	while (in >> command) {
		if (command == "addx") {
			int val;
			in >> val;
			cycleNumber++;
			sumOfInterestingSignalStrengths += interestingSignalStrenths(regX, cycleNumber);
			cycleNumber++;
			sumOfInterestingSignalStrengths += interestingSignalStrenths(regX, cycleNumber);
			regX += val;
		} else if (command == "noop") {
			cycleNumber++;
			sumOfInterestingSignalStrengths += interestingSignalStrenths(regX, cycleNumber);
		}
	}

	std::cout << sumOfInterestingSignalStrengths << "\n";

	return 0;
}

class CRT {
	static constexpr int pixelsInRow = 40;
	static constexpr int rows = 6;
public:
	CRT() : headPosition(0), spritePosition(1) {
		std::fill(screen.begin(), screen.end(), ' ');
	}

	void printScreen() const {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < pixelsInRow; j++) {
				std::cout << screen.at(i * pixelsInRow + j);
			}
			std::cout << "\n";
		}
	}

	void addCommand(int val) {
		tick();
		tick();
		spritePosition += val;
	}

	void noopCommand() {
		tick();
	}

private:
	void tick() {
		screen.at(headPosition) = getPixelAtHead();
		headPosition++;
	}

	char getPixelAtHead() const {
		if (spriteOverlapsHead()) {
			return '#';
		} else {
			return '.';
		}
	}

	bool spriteOverlapsHead() const {
		int headInRow = headPosition % pixelsInRow;
		return headInRow >= spritePosition - 1 && headInRow <= spritePosition + 1;
	}

	int headPosition;
	int spritePosition;

	std::array<char, pixelsInRow * rows> screen;
};

int f2(std::istream& in) {
	CRT crt;
	std::string command;
	while (in >> command) {
		if (command == "addx") {
			int val;
			in >> val;
			crt.addCommand(val);
		} else if (command == "noop") {
			crt.noopCommand();
		}
	}

	crt.printScreen();

	return 0;
}

int main(int argc, const char* argv[]) {
	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
	return 1;
}