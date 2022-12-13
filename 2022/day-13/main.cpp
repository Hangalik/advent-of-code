#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <set>

#include "input_selector.h"

class PacketDataVisitor;

class PacketData {
public:
	virtual void visit(PacketDataVisitor& visitor) const = 0;
};

class IntegerData;
class ListData;

class PacketDataVisitor {
public:
	virtual void accept(const IntegerData& data) = 0;
	virtual void accept(const ListData& data) = 0;
};

class IntegerData : public PacketData {
public:
	explicit IntegerData(int value) : data(value) {}

	void visit(PacketDataVisitor& visitor) const override {
		visitor.accept(*this);
	}

	friend int compare(const IntegerData& id1, const IntegerData& id2);
	friend int compare(const ListData& ld1, const IntegerData& id2);

private:
	int data;
};

class ListData : public PacketData {
public:
	void visit(PacketDataVisitor& visitor) const override {
		visitor.accept(*this);
	}

	friend int compare(const ListData& ld1, const ListData& ld2);
	friend int compare(const ListData& ld1, const IntegerData& id2);

	void addElement(std::shared_ptr<PacketData> element) {
		elements.push_back(std::move(element));
	}

private:
	std::vector<std::shared_ptr<PacketData>> elements;
};

int compare(const PacketData& pd1, const PacketData& pd2);
int compare(const IntegerData& id1, const IntegerData& id2);
int compare(const ListData& ld1, const ListData& ld2);
int compare(const ListData& ld1, const IntegerData& id2);
int compare(const IntegerData& id1, const ListData& ld2);

class CompareListVisitor : public PacketDataVisitor {
public:
	explicit CompareListVisitor(const ListData& listData) : ld(&listData) {}

	void accept(const IntegerData& data) override {
		result = compare(data, *ld);
	}

	void accept(const ListData& data) override {
		result = compare(data, *ld);
	}

	int result;

private:
	const ListData* ld;
};

class CompareIntegerVisitor : public PacketDataVisitor {
public:
	explicit CompareIntegerVisitor(const IntegerData& integerData) : id(&integerData) {}

	void accept(const IntegerData& data) override {
		result = compare(data, *id);
	}

	void accept(const ListData& data) override {
		result = compare(data, *id);
	}

	int result;

private:
	const IntegerData* id;
};

class CompareVisitor : public PacketDataVisitor {
public:
	explicit CompareVisitor(const PacketData& packetData) : pd(&packetData) {}

	void accept(const IntegerData& data) override {
		CompareIntegerVisitor civ(data);
		pd->visit(civ);
		result = civ.result;
	}

	void accept(const ListData& data) override {
		CompareListVisitor clv(data);
		pd->visit(clv);
		result = clv.result;
	}

	int result;

private:
	const PacketData* pd;
};

int compare(const PacketData& pd1, const PacketData& pd2) {
	CompareVisitor cv(pd1);
	pd2.visit(cv);
	return cv.result;
}

int compare(const IntegerData& id1, const IntegerData& id2) {
	if (id1.data < id2.data) {
		return -1;
	}
	if (id1.data > id2.data) {
		return 1;
	}
	return 0;
}

int compare(const ListData& ld1, const ListData& ld2) {
	size_t smaller = std::min(ld1.elements.size(), ld2.elements.size());
	for (size_t i = 0; i < smaller; i++) {
		int elementResult = compare(*ld1.elements[i], *ld2.elements[i]);
		if (elementResult != 0) {
			return elementResult;
		}
	}
	if (ld1.elements.size() < ld2.elements.size()) {
		return -1;
	}
	if (ld1.elements.size() > ld2.elements.size()) {
		return 1;
	}
	return 0;
}

int compare(const ListData& ld1, const IntegerData& id2) {
	if (ld1.elements.empty()) {
		return -1;
	}
	CompareIntegerVisitor civ(id2);
	ld1.elements.front()->visit(civ);
	if (civ.result != 0) {
		return civ.result;
	}
	if (ld1.elements.size() > 1) {
		return 1;
	}
	return 0;
}

int compare(const IntegerData& id1, const ListData& ld2) {
	return -compare(ld2, id1);
}

std::pair<std::shared_ptr<PacketData>, std::string::const_iterator> readInPacket(
	std::string::const_iterator begin, std::string::const_iterator end)
{
	if (begin == end) {
		throw std::logic_error("cannot read in packet");
	}
	char c = *begin;
	if (std::isdigit(c)) {
		int val = c - '0';
		if (begin + 1 != end) {
			char cc = *(begin + 1);
			if (std::isdigit(cc)) {
				begin++;
				val *= 10;
				val += cc - '0';
			}
		}
		return {std::make_shared<IntegerData>(val), begin + 1};
	}
	if (c == '[') {
		auto list = std::make_shared<ListData>();
		begin++;
		while (true) {
			if (*begin == ']') {
				break;
			}
			if (*begin == ',') {
				begin++;
			}
			auto [element, afterElement] = readInPacket(begin, end);
			list->addElement(element);
			begin = afterElement;
		}
		return {list, begin + 1};
	}
	throw std::logic_error("cannot read in packet");
}

std::shared_ptr<PacketData> readInPacket(const std::string& line) {
	auto [result, ignore] = readInPacket(line.begin(), line.end());
	return result;
}

int f1(std::istream& in) {
	std::string line;
	int sumOfCorrectIndices = 0;
	int idx = 0;
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		idx++;
		auto p1 = readInPacket(line);
		std::getline(in, line);
		auto p2 = readInPacket(line);

		int comp = compare(*p1, *p2);
		if (comp < 0) {
			sumOfCorrectIndices += idx;
		}
	}

	std::cout << sumOfCorrectIndices << "\n";

	return 0;
}

struct PacketComparator {
	bool operator()(const std::shared_ptr<PacketData>& pd1, const std::shared_ptr<PacketData>& pd2) const {
		int result = compare(*pd1, *pd2);
		return result < 0;
	}
};

int f2(std::istream& in) {
	std::string line;
	std::set<std::shared_ptr<PacketData>, PacketComparator> packets;
	auto divider1 = readInPacket("[[2]]");
	auto divider2 = readInPacket("[[6]]");
	packets.insert(divider1);
	packets.insert(divider2);
	while (std::getline(in, line)) {
		if (line.empty()) {
			continue;
		}
		packets.insert(readInPacket(line));
	}

	int divider1Idx = 0, divider2Idx = 0;
	int idx = 0;
	for (const auto& packet : packets) {
		idx++;
		if (packet == divider1) {
			divider1Idx = idx;
		} else if (packet == divider2) {
			divider2Idx = idx;
		}
	}

	std::cout << divider1Idx * divider2Idx << "\n";

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