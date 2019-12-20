#include <iostream>
#include <vector>

int main() {

	std::vector<int> intCode;

	int next;
	while (std::cin >> next) {
		intCode.emplace_back(next);
	}
	intCode[1] = 12;
	intCode[2] = 2;

	int i{ 0 };
	while (true) {
		if (intCode.at(i) == 99) {
			std::cout << "intCode[0] = " << intCode[0] << std::endl;
			return 0;
		}

		const int lhs = intCode.at(intCode.at(i + 1));
		const int rhs = intCode.at(intCode.at(i + 2));
		const int resultPos = intCode.at(i + 3);
		switch (intCode.at(i)) {
		case 1:
			intCode.at(resultPos) = lhs + rhs;
			break;
		case 2:
			intCode.at(resultPos) = lhs * rhs;
			break;
		default:
			return 1;
		}
		i += 4;
	}

	return 0;
}

