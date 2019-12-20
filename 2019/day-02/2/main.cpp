#include <iostream>
#include <vector>

std::vector<long long> readVector() {
	std::vector<long long> intCode;
	long long next;
	while (std::cin >> next) {
		intCode.emplace_back(next);
	}
	return intCode;
}

long long compute(std::vector<long long>& intCode) {
	int i{ 0 };
	while (true) {
		if (intCode.at(i) == 99) {
			return intCode[0];
		}

		const long long lhs = intCode.at(intCode.at(i + 1));
		const long long rhs = intCode.at(intCode.at(i + 2));
		const long long resultPos = intCode.at(i + 3);
		switch (intCode.at(i)) {
		case 1:
			intCode.at(resultPos) = lhs + rhs;
			break;
		case 2:
			intCode.at(resultPos) = lhs * rhs;
			break;
		default:
			throw "Unknown op code: " + intCode[i];
		}
		i += 4;
	}
}

int main() {

	const std::vector<long long> intCode{ readVector() };

	for (long long noun = 0; noun < 100; noun++) {
		for (long long verb = 0; verb < 100; verb++) {
			std::vector<long long> iterCode{ intCode };
			iterCode[1] = noun;
			iterCode[2] = verb;
			long long result = compute(iterCode);
			if (result == 19690720) {
				std::cout << "100 * noun + verb = " << noun * 100 + verb << std::endl;
				// break;
			}
		}
	}

	return 0;
}

