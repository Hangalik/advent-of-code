#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include "input_selector.h"

class Pair;

class PairElement {
public:
	explicit PairElement(int value) : value(value), pair() {}
	explicit PairElement(std::unique_ptr<Pair>&& pair) : value(-1), pair(std::move(pair)) {}

	PairElement(const PairElement& other) {
		if (other.isValue()) {
			value = other.value;
			return;
		}
		pair = std::make_unique<Pair>(*other.getPair());
	}

	bool isValue() const {
		return !((bool) pair);
	}

	Pair* getPair() const {
		if (isValue()) {
			throw std::logic_error("PairElement holds value");
		}
		return pair.get();
	}

	int getValue() const {
		if (!isValue()) {
			throw std::logic_error("PairElement holds pair");
		}
		return value;
	}

	void print(std::ostream& os) const;
	int magnitude() const;

	void increaseValue(int val) {
		if (!isValue()) {
			throw std::logic_error("PairElement holds pair");
		}
		value += val;
	}

	bool explode(PairElement* leftNeighbour, PairElement* rightNeighbour, int level);
	bool split();

	PairElement* getLeftMostValue();
	PairElement* getRightMostValue();

private:
	int value;
	std::unique_ptr<Pair> pair;
};

class Pair {
	static PairElement dummyPe;
public:
	explicit Pair(PairElement&& left, PairElement&& right) : left(std::move(left)), right(std::move(right)) {}

	int magnitude() const {
		return 3 * left.magnitude() + 2 * right.magnitude();
	}

	void reduce() {
		while (true) {
			bool exploded = explode(&dummyPe, &dummyPe, 1);
			if (exploded) continue;

			bool splitted = split();
			if (!splitted) break;
		}
	}

	PairElement* getLeftMostValue() {
		return left.getLeftMostValue();
	}

	PairElement* getRightMostValue() {
		return right.getRightMostValue();
	}

	void print(std::ostream& os) const {
		os << "[";
		left.print(os);
		os << ",";
		right.print(os);
		os << "]";
	}

	bool explode(PairElement* leftNeighbour, PairElement* rightNeighbour, int level) {
		if (left.explode(leftNeighbour, right.getLeftMostValue(), level + 1)) {
			return true;
		}
		if (right.explode(left.getRightMostValue(), rightNeighbour, level + 1)) {
			return true;
		}
		return false;
	}

	bool split() {
		if (left.split()) {
			return true;
		}
		if (right.split()) {
			return true;
		}
		return false;
	}

private:
	PairElement left;
	PairElement right;
};

PairElement Pair::dummyPe(0);

void PairElement::print(std::ostream& os) const {
	if (isValue()) {
		os << value;
	} else {
		pair->print(os);
	}
}

int PairElement::magnitude() const {
	if (isValue()) {
		return value;
	}
	return pair->magnitude();
}

PairElement* PairElement::getLeftMostValue() {
	if (isValue()) {
		return this;
	}
	return pair->getLeftMostValue();
}

PairElement* PairElement::getRightMostValue() {
	if (isValue()) {
		return this;
	}
	return pair->getRightMostValue();
}

bool PairElement::explode(PairElement* leftNeighbour, PairElement* rightNeighbour, int level) {
	if (isValue()) {
		return false;
	}
	if (level < 5) {
		return pair->explode(leftNeighbour, rightNeighbour, level);
	}
	leftNeighbour->increaseValue(pair->getLeftMostValue()->value);
	rightNeighbour->increaseValue(pair->getRightMostValue()->value);
	pair.reset();
	value = 0;
	return true;
}

bool PairElement::split() {
	if (!isValue()) {
		return pair->split();
	}
	if (value > 9) {
		int lowerHalf = value / 2;
		PairElement newLeft(lowerHalf);
		PairElement newRight(value - lowerHalf);
		pair = std::make_unique<Pair>(std::move(newLeft), std::move(newRight));
		return true;
	}
	return false;
}

std::unique_ptr<Pair> readPair(std::istream& in);

PairElement readPairElement(std::istream& in) {
	char c;
	in >> c;
	if (c == '[') {
		return PairElement{readPair(in)};
	} else if (std::isdigit(c)) {
		return PairElement{c - '0'};
	} else {
		throw std::logic_error("unexpected character: " + c);
	}
}

std::unique_ptr<Pair> readPair(std::istream& in) {
	//in.ignore(); 	// "["
	PairElement left = readPairElement(in);
	in.ignore();	// ","
	PairElement right = readPairElement(in);
	in.ignore();	// "]"
	return std::make_unique<Pair>(std::move(left), std::move(right));
}

std::ostream& operator<<(std::ostream& os, const Pair& pair) {
	pair.print(os);
	return os;
}

int f1(std::istream& in) {
	std::string line;
	std::unique_ptr<Pair> previousPair;
	while (std::getline(in, line, '\n')) {
		std::istringstream iss(line);
		iss.ignore();	// Starting "["
		std::unique_ptr<Pair> p = readPair(iss);
		if (previousPair) {
			PairElement left(std::move(previousPair));
			PairElement right(std::move(p));
			previousPair = std::make_unique<Pair>(std::move(left), std::move(right));
			previousPair->reduce();
		} else {
			previousPair = std::move(p);
		}
	}
	//std::cout << *previousPair << std::endl;;
	std::cout << previousPair->magnitude() << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::vector<std::unique_ptr<Pair>> pairs;
	std::string line;
	while (std::getline(in, line, '\n')) {
		std::istringstream iss(line);
		iss.ignore();	// Starting "["
		std::unique_ptr<Pair> p = readPair(iss);
		pairs.push_back(std::move(p));
	}

	std::set<int, std::less<int>> magnitudes;
	for (size_t i = 0; i < pairs.size(); i++) {
		for (size_t j = 0; j < pairs.size(); j++) {
			if (i == j) continue;
			PairElement left{std::make_unique<Pair>(*pairs[i])};
			PairElement right{std::make_unique<Pair>(*pairs[j])};
			Pair p{std::move(left), std::move(right)};
			p.reduce();
			magnitudes.insert(p.magnitude());
		}
	}

	std::cout << *magnitudes.rbegin() << std::endl;

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
