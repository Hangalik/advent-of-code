#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

#include "input_selector.h"

uint8_t hexToBits(char c) {
	if (c >= 'A') {
		return c - 'A' + 10;
	} else {
		return c - '0';
	}
}

std::vector<uint8_t> hexaParser(const std::string& hexaString) {
	std::vector<uint8_t> result;
	size_t i = 0;
	while (i < hexaString.size()) {
		uint8_t val = hexToBits(hexaString[i]) << 4;
		i++;
		if (i < hexaString.size()) {
			val |= hexToBits(hexaString[i]);
			i++;
		}
		result.push_back(val);
	}
	return result;
}

class BitParser {
public:
	BitParser(const std::string& hexaString) : data(hexaParser(hexaString)), bitPosition(0) {}

	int getInt(size_t bits) {
		int val = 0;
		for (size_t i = 0; i < bits; i++) {
			val *= 2;
			if (getBit(i)) {
				val += 1;
			}
		}
		bitPosition += bits;
		return val;
	}

	void ignore(size_t bits) {
		if (bits > 0) {
			getBit(bits - 1);	// Check if it is a valid position
			bitPosition += bits;
		}
	}

	size_t remaining() const {
		size_t allBitCount = data.size() * 8;
		if (bitPosition >= allBitCount) {
			return 0;
		}
		return allBitCount - bitPosition;
	}

private:
	bool getBit(size_t pos) const {
		size_t idx = getIdx(pos);
		uint8_t mask = maskForPos(pos);
		uint8_t bit = data.at(idx) & mask;
		return bit != 0;
	}

	size_t getIdx(size_t pos) const {
		return (bitPosition + pos) / 8;
	}

	uint8_t maskForPos(size_t pos) const {
		size_t posInByte = (bitPosition + pos) % 8;
		return 1 << (7 - posInByte);
	}

	std::vector<uint8_t> data;
	size_t bitPosition;
};

static constexpr int SUM_TYPE = 0;
static constexpr int PRODUCT_TYPE = 1;
static constexpr int MINIMUM_TYPE = 2;
static constexpr int MAXIMUM_TYPE = 3;
static constexpr int LITERAL_TYPE = 4;
static constexpr int GREATER_TYPE = 5;
static constexpr int LESSER_TYPE = 6;
static constexpr int EQUAL_TYPE = 7;

struct Packet {
	int version;
	int typeId;
	int value;
};

int getSumOfVersions(BitParser& bp) {
	int version = bp.getInt(3);
	int typeId = bp.getInt(3);
	if (typeId == LITERAL_TYPE) {
		int value = 0;
		bool hasMore = true;
		while (hasMore) {
			hasMore = bp.getInt(1) == 1;
			value *= 16;
			value += bp.getInt(4);
		}
		return version;
	} else {
		int lengthType = bp.getInt(1);
		int versionSum = version;
		if (lengthType == 1) {
			int subPacketCount = bp.getInt(11);
			for (int i = 0; i < subPacketCount; i++) {
				versionSum += getSumOfVersions(bp);
			}
		} else {
			int bitsOfSubPackets = bp.getInt(15);
			size_t goalRemaining = bp.remaining() - bitsOfSubPackets;
			while (bp.remaining() > goalRemaining) {
				versionSum += getSumOfVersions(bp);
			}
		}
		return versionSum;
	}
}

int f1(std::istream& in) {
	std::string line;
	in >> line;
	BitParser bp(line);

	int versionSum = getSumOfVersions(bp);
	
	std::cout << versionSum << std::endl;

	return 0;
}

long long packetSummer(const std::vector<long long>& values) {
	return std::accumulate(values.begin(), values.end(), 0LL, std::plus<long long>());
}

long long packetMultiplier(const std::vector<long long>& values) {
	return std::accumulate(values.begin(), values.end(), 1LL, std::multiplies<long long>());
}

long long packetMinner(const std::vector<long long>& values) {
	auto minner = [](long long a, long long b) { return std::min(a, b); };
	constexpr long long minInit = std::numeric_limits<long long>::max();
	return std::accumulate(values.begin(), values.end(), minInit, minner);
}

long long packetMaxer(const std::vector<long long>& values) {
	auto maxer = [](long long a, long long b) { return std::max(a, b); };
	constexpr long long maxInit = std::numeric_limits<long long>::min();
	return std::accumulate(values.begin(), values.end(), maxInit, maxer);
}

long long packetGreater(const std::vector<long long>& values) {
	return values[0] > values[1];
}

long long packetLesser(const std::vector<long long>& values) {
	return values[0] < values[1];
}

long long packetEqualer(const std::vector<long long>& values) {
	return values[0] == values[1];
}

std::function<long long(const std::vector<long long>&)> getOperator(int typeId) {
	switch (typeId) {
	case SUM_TYPE:
		return packetSummer;
	case PRODUCT_TYPE:
		return packetMultiplier;
	case MINIMUM_TYPE:
		return packetMinner;
	case MAXIMUM_TYPE:
		return packetMaxer;
	case GREATER_TYPE:
		return packetGreater;
	case LESSER_TYPE:
		return packetLesser;
	case EQUAL_TYPE:
		return packetEqualer;
	default:
		throw std::invalid_argument("invalid typeId: " + typeId);
	}
}

char getOperatorChar(int typeId) {
	switch (typeId) {
	case SUM_TYPE:
		return '+';
	case PRODUCT_TYPE:
		return '*';
	case MINIMUM_TYPE:
		return 'i';
	case MAXIMUM_TYPE:
		return 'a';
	case GREATER_TYPE:
		return '>';
	case LESSER_TYPE:
		return '<';
	case EQUAL_TYPE:
		return '=';
	default:
		throw std::invalid_argument("invalid typeId: " + typeId);
	}
}

long long getValueOfExpression(BitParser& bp) {
	bp.getInt(3); // version
	int typeId = bp.getInt(3);
	if (typeId == LITERAL_TYPE) {
		long long value = 0;
		bool hasMore = true;
		while (hasMore) {
			hasMore = bp.getInt(1) == 1;
			value *= 16;
			value += bp.getInt(4);
		}
		//std::cout << value << std::endl;
		return value;
	} else {
		std::vector<long long> values;
		int lengthType = bp.getInt(1);
		//std::cout << getOperatorChar(typeId) << " (";
		if (lengthType == 1) {
			int subPacketCount = bp.getInt(11);
			for (int i = 0; i < subPacketCount; i++) {
				long long value = getValueOfExpression(bp);
				values.push_back(value);
			}
		} else {
			int bitsOfSubPackets = bp.getInt(15);
			size_t goalRemaining = bp.remaining() - bitsOfSubPackets;
			while (bp.remaining() > goalRemaining) {
				long long value = getValueOfExpression(bp);
				values.push_back(value);
			}
		}
		auto operation = getOperator(typeId);
		long long result = operation(values);
		//std::cout << ") = " << result << std::endl;
		return result;
	}
}

int f2(std::istream& in) {
	std::string line;
	in >> line;
	BitParser bp(line);

	long long versionSum = getValueOfExpression(bp);

	std::cout << versionSum << std::endl;

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
