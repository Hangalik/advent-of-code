#include <iostream>
#include <iomanip>
#include <vector>

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

static constexpr int LITERAL_TYPE = 4;

struct Packet {
	int version;
	int typeId;
	int value;
};

int f1(std::istream& in) {
	std::string line;
	in >> line;
	BitParser bp(line);

	int version = bp.getInt(3);
	int typeId = bp.getInt(3);
	if (typeId != LITERAL_TYPE) {
		std::cout << "not literal type" << std::endl;
		return 0;
	}
	int value = 0;
	bool hasMore = true;
	while (hasMore) {
		hasMore = bp.getInt(1) == 1;
		for (int i = 0; i < 4; i++) {
			value *= 2;
			value += bp.getInt(1);
		}
	}
	std::cout << value << std::endl;

	return 0;
}

int f2(std::istream& ) {

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
}
