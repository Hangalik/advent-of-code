#include <iostream>
#include <vector>

constexpr int BUFFER_SIZE = 16;

std::vector<long long> readVector() {
	std::vector<long long> intCode;
	char buffer[BUFFER_SIZE];
	while (std::cin.getline(buffer, BUFFER_SIZE, ',')) {
		auto next{ std::strtoll(buffer, nullptr, 10) };
		intCode.emplace_back(next);
	}
	return intCode;
}

long long parameterMode(long long opCode, long long numberOfParameter) {
	opCode /= 100;
	for (int i = 0; i < numberOfParameter - 1; i++) {
		opCode /= 10;
	}
	return opCode % 10;
}

long long getParameter(long long parameterMode, long long parameter, const std::vector<long long>& intCode) {
	switch (parameterMode) {
	case 0:
		return intCode.at(parameter);
	case 1:
		return parameter;
	default:
		throw "Unknown parameter mode: " + parameterMode;
	}
}

int inputReads = 0;

long long readInput() {
	inputReads++;
	if (inputReads > 1) {
		std::cout << "More than 1 input reads!" << std::endl;
	}
	return 5ll;
}

void compute(std::vector<long long>& intCode) {
	long long i{ 0 };
	while (true) {
		switch (intCode.at(i) % 100) {
		case 1:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			const long long resultPos = intCode.at(i + 3);
			intCode.at(resultPos) = lhs + rhs;
			i += 4;
			break;
		}
		case 2:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			const long long resultPos = intCode.at(i + 3);
			intCode.at(resultPos) = lhs * rhs;
			i += 4;
			break;
		}
		case 3:
		{
			const long long resultPos = intCode.at(i + 1);
			intCode.at(resultPos) = readInput();
			i += 2;
			break;
		}
		case 4:
		{
			const long long pos = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			std::cout << pos << std::endl;
			i += 2;
			break;
		}
		case 5:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			if (lhs != 0) {
				i = rhs;
			} else {
				i += 3;
			}
			break;
		}
		case 6:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			if (lhs == 0) {
				i = rhs;
			} else {
				i += 3;
			}
			break;
		}
		case 7:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			const long long resultPos = intCode.at(i + 3);
			if (lhs < rhs) {
				intCode.at(resultPos) = 1;
			} else {
				intCode.at(resultPos) = 0;
			}
			i += 4;
			break;
		}
		case 8:
		{
			const long long lhs = getParameter(parameterMode(intCode[i], 1), intCode.at(i + 1), intCode);
			const long long rhs = getParameter(parameterMode(intCode[i], 2), intCode.at(i + 2), intCode);
			const long long resultPos = intCode.at(i + 3);
			if (lhs == rhs) {
				intCode.at(resultPos) = 1;
			} else {
				intCode.at(resultPos) = 0;
			}
			i += 4;
			break;
		}
		case 99:
			return;
		default:
			throw "Unknown op code: " + intCode[i];
		}
	}
}

int main() {

	std::vector<long long> intCode{ readVector() };

	compute(intCode);

	return 0;
}

