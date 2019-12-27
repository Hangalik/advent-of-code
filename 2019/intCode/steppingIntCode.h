#include <istream>
#include <vector>
#include <deque>

enum class ComputationProgress {
	WAITING_FOR_INPUT,
	TERMINATED,
	ERROR
};

constexpr int BUFFER_SIZE = 16;

class SteppingIntCode {
public:
	explicit SteppingIntCode(std::istream& intCodeSource, std::deque<int>& input, std::deque<int>& output) : programCounter{ 0 }, input(input), output(output) {
		char buffer[BUFFER_SIZE];
		while (intCodeSource.getline(buffer, BUFFER_SIZE, ',')) {
			int next{ std::stoi(buffer) };
			intCode.emplace_back(next);
		}
	}

	explicit SteppingIntCode(const SteppingIntCode& other, std::deque<int>& input, std::deque<int>& output) : intCode(other.intCode), programCounter{ 0 }, input(input), output(output)
	{}

	ComputationProgress step() {
		while (true) {
			switch (intCode.at(programCounter) % 100) {
			case 1:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				const int resultPos = intCode.at(programCounter + 3);
				intCode.at(resultPos) = lhs + rhs;
				programCounter += 4;
				break;
			}
			case 2:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				const int resultPos = intCode.at(programCounter + 3);
				intCode.at(resultPos) = lhs * rhs;
				programCounter += 4;
				break;
			}
			case 3:
			{
				if (input.empty()) {
					return ComputationProgress::WAITING_FOR_INPUT;
				}
				const int resultPos = intCode.at(programCounter + 1);
				intCode.at(resultPos) = input.front();
				input.pop_front();
				programCounter += 2;
				break;
			}
			case 4:
			{
				const int pos = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				output.push_back(pos);
				programCounter += 2;
				break;
			}
			case 5:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				if (lhs != 0) {
					programCounter = rhs;
				} else {
					programCounter += 3;
				}
				break;
			}
			case 6:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				if (lhs == 0) {
					programCounter = rhs;
				} else {
					programCounter += 3;
				}
				break;
			}
			case 7:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				const int resultPos = intCode.at(programCounter + 3);
				if (lhs < rhs) {
					intCode.at(resultPos) = 1;
				} else {
					intCode.at(resultPos) = 0;
				}
				programCounter += 4;
				break;
			}
			case 8:
			{
				const int lhs = getParameter(intCode[programCounter], 1, intCode.at(programCounter + 1), intCode);
				const int rhs = getParameter(intCode[programCounter], 2, intCode.at(programCounter + 2), intCode);
				const int resultPos = intCode.at(programCounter + 3);
				if (lhs == rhs) {
					intCode.at(resultPos) = 1;
				} else {
					intCode.at(resultPos) = 0;
				}
				programCounter += 4;
				break;
			}
			case 99:
				return ComputationProgress::TERMINATED;
			default:
				std::cout << "Unknown opCode: " << intCode[programCounter] << " at " << programCounter << std::endl;
				return ComputationProgress::ERROR;
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
	int programCounter;
	std::deque<int>& input;
	std::deque<int>& output;
};
