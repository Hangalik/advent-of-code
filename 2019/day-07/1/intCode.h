#include <istream>
#include <vector>

constexpr int BUFFER_SIZE = 16;

class IntCode {
public:
	explicit IntCode(std::istream& intCodeSource) {
		char buffer[BUFFER_SIZE];
		while (intCodeSource.getline(buffer, BUFFER_SIZE, ',')) {
			int next{ std::stoi(buffer) };
			intCode.emplace_back(next);
		}
	}

	void compute(const std::vector<int>& input, std::vector<int>& output) {
		int i{ 0 };
		auto inputIterator{ input.cbegin() };
		while (true) {
			switch (intCode.at(i) % 100) {
			case 1:
			{
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				const int resultPos = intCode.at(i + 3);
				intCode.at(resultPos) = lhs + rhs;
				i += 4;
				break;
			}
			case 2:
			{
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				const int resultPos = intCode.at(i + 3);
				intCode.at(resultPos) = lhs * rhs;
				i += 4;
				break;
			}
			case 3:
			{
				const int resultPos = intCode.at(i + 1);
				intCode.at(resultPos) = *inputIterator;
				inputIterator++;
				i += 2;
				break;
			}
			case 4:
			{
				const int pos = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				output.push_back(pos);
				i += 2;
				break;
			}
			case 5:
			{
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				if (lhs != 0) {
					i = rhs;
				} else {
					i += 3;
				}
				break;
			}
			case 6:
			{
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				if (lhs == 0) {
					i = rhs;
				} else {
					i += 3;
				}
				break;
			}
			case 7:
			{
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				const int resultPos = intCode.at(i + 3);
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
				const int lhs = getParameter(intCode[i], 1, intCode.at(i + 1), intCode);
				const int rhs = getParameter(intCode[i], 2, intCode.at(i + 2), intCode);
				const int resultPos = intCode.at(i + 3);
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
	
private:
	int parameterMode(int opCode, int numberOfParameter) const {
		opCode /= 100;
		for (int i = 0; i < numberOfParameter - 1; i++) {
			opCode /= 10;
		}
		return opCode % 10;
	}

	int getParameter(int opCode, int numberOfParameter, int parameter, const std::vector<int>& intCode) const {
		int parameterMode = this->parameterMode(opCode, numberOfParameter);
		switch (parameterMode) {
		case 0:
			return intCode.at(parameter);
		case 1:
			return parameter;
		default:
			throw "Unknown parameter mode: " + parameterMode;
		}
	}

	std::vector<int> intCode;
};
