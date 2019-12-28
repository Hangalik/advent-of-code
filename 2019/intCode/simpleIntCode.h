#include <istream>
#include <deque>
#include <vector>

constexpr int BUFFER_SIZE = 32;

class SimpleIntCode {
public:
	using value_type = long long;

	explicit SimpleIntCode(std::istream& intCodeSource) : relativeBase{ 0 } {
		char buffer[BUFFER_SIZE];
		while (intCodeSource.getline(buffer, BUFFER_SIZE, ',')) {
			value_type next{ std::stoll(buffer) };
			intCode.push_back(next);
		}
	}

	void compute(const std::vector<value_type>& input, std::vector<value_type>& output) {
		int i{ 0 };
		auto inputIterator{ input.cbegin() };
		while (true) {
			switch (intCodeAt(i) % 100) {
			case 1:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				const value_type resultPos = getParameterPosition(i, 3);
				intCodeAt(resultPos) = intCodeAt(lhs) + intCodeAt(rhs);
				i += 4;
				break;
			}
			case 2:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				const value_type resultPos = getParameterPosition(i, 3);
				intCodeAt(resultPos) = intCodeAt(lhs) * intCodeAt(rhs);
				i += 4;
				break;
			}
			case 3:
			{
				const value_type resultPos = getParameterPosition(i, 1);
				intCodeAt(resultPos) = *inputIterator;
				inputIterator++;
				i += 2;
				break;
			}
			case 4:
			{
				const value_type pos = getParameterPosition(i, 1);
				output.push_back(intCodeAt(pos));
				i += 2;
				break;
			}
			case 5:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				if (intCodeAt(lhs) != 0) {
					i = intCodeAt(rhs);
				} else {
					i += 3;
				}
				break;
			}
			case 6:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				if (intCodeAt(lhs) == 0) {
					i = intCodeAt(rhs);
				} else {
					i += 3;
				}
				break;
			}
			case 7:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				const value_type resultPos = getParameterPosition(i, 3);
				if (intCodeAt(lhs) < intCodeAt(rhs)) {
					intCodeAt(resultPos) = 1;
				} else {
					intCodeAt(resultPos) = 0;
				}
				i += 4;
				break;
			}
			case 8:
			{
				const value_type lhs = getParameterPosition(i, 1);
				const value_type rhs = getParameterPosition(i, 2);
				const value_type resultPos = getParameterPosition(i, 3);
				if (intCodeAt(lhs) == intCodeAt(rhs)) {
					intCodeAt(resultPos) = 1;
				} else {
					intCodeAt(resultPos) = 0;
				}
				i += 4;
				break;
			}
			case 9:
			{
				const value_type lhs = getParameterPosition(i, 1);
				relativeBase += intCodeAt(lhs);
				i += 2;
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
	value_type& intCodeAt(value_type position) {
		if (position < 0) {
			throw "indexing with negative index";
		}
		auto pos = static_cast<typename std::deque<value_type>::size_type>(position);
		if (intCode.size() < pos + 1) {
			intCode.resize(pos + 1);
		}
		return intCode[pos];
	}

	value_type parameterMode(value_type opCode, int numberOfParameter) const {
		opCode /= 100;
		for (int i = 0; i < numberOfParameter - 1; i++) {
			opCode /= 10;
		}
		return opCode % 10;
	}

	value_type getParameterPosition(int opCodePosition, int numberOfParameter) {
		value_type opCode{ intCode.at(opCodePosition) };
		value_type parameterMode = this->parameterMode(opCode, numberOfParameter);
		switch (parameterMode) {
		case 0:
			return intCodeAt(opCodePosition + numberOfParameter);
		case 1:
			return opCodePosition + numberOfParameter;
		case 2:
			return relativeBase + intCodeAt(opCodePosition + numberOfParameter);
		default:
			throw "Unknown parameter mode: " + parameterMode;
		}
	}

	std::deque<value_type> intCode;
	value_type relativeBase;
};
