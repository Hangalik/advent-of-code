#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <memory>
#include <queue>
#include <functional>
#include <limits>

#include "input_selector.h"

class Monkey {
	size_t monkey_num;
public:
	explicit Monkey(long long boredomFactor, size_t monkey_num) : monkey_num(monkey_num), boredomFactor(boredomFactor) {}

	void receiveItem(long long level) {
		items.push(level % commonDivider);
	}

	void setCommonDivider(long long divider) {
		commonDivider = divider;
	}

	void setOperation(std::function<long long(long long)> op) {
		operation = std::move(op);
	}

	void setDivider(int div) {
		divisibleTest = div;
	}

	void setTrueTarget(std::weak_ptr<Monkey> target) {
		trueTarget = std::move(target);
	}

	void setFalseTarget(std::weak_ptr<Monkey> target) {
		falseTarget = std::move(target);
	}

	void takeTurn() {
		while (!items.empty()) {
			itemsInspected++;
			long long nextItem = items.front();
			items.pop();
			throwItem(nextItem);
		}
	}

	long long getItemsInspected() const {
		return itemsInspected;
	}

	std::queue<long long, std::list<long long>> getItems() const {
		return items;
	}

private:
	void throwItem(long long val) const {
		long long newVal = operation(val);
		long long boredVal = newVal / boredomFactor;
		if (boredVal % divisibleTest == 0) {
			throwTo(boredVal, trueTarget);
		} else {
			throwTo(boredVal, falseTarget);
		}
	}

	void throwTo(long long val, const std::weak_ptr<Monkey>& target) const {
		if (std::shared_ptr<Monkey> targetShared = target.lock()) {
			targetShared->receiveItem(val);
		}
	}

	std::queue<long long, std::list<long long>> items;
	long long commonDivider = std::numeric_limits<long long>::max();

	std::function<long long(long long)> operation;

	long long boredomFactor;

	long long divisibleTest;

	std::weak_ptr<Monkey> trueTarget;
	std::weak_ptr<Monkey> falseTarget;

	long long itemsInspected = 0;
};

void ensureSize(std::vector<std::shared_ptr<Monkey>>& monkeys, size_t size, int boredomFactor) {
	while (monkeys.size() < size) {
		monkeys.push_back(std::make_shared<Monkey>(boredomFactor, monkeys.size()));
	}
}

void skipLine(std::istream& in) {
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::function<long long(long long)> bindParameters(auto operation, const std::string& lhs, const std::string& rhs) {
	using namespace std::placeholders;
	if (lhs == "old") {
		if (rhs == "old") {
			return std::bind(operation, _1, _1);
		} else {
			return std::bind(operation, _1, std::stoi(rhs));
		}
	} else {
		if (rhs == "old") {
			return std::bind(operation, std::stoi(lhs), _1);
		} else {
			return std::bind(operation, std::stoi(lhs), std::stoi(rhs));
		}
	}
}

std::function<long long(long long)> parseOperation(std::istream& in) {
	std::string lhs, op, rhs;
	in >> lhs >> op >> rhs;
	if (op == "+") {
		return bindParameters(std::plus<long long>{}, lhs, rhs);
	} else if (op == "*") {
		return bindParameters(std::multiplies<long long>{}, lhs, rhs);
	}
	throw std::logic_error("no such operation");
}

void printMonkeyItems(std::shared_ptr<Monkey> monkey, size_t num) {
	std::cout << "Monkey " << num << ": ";
	auto items = monkey->getItems();
	while (!items.empty()) {
		std::cout << items.front() << ", ";
		items.pop();
	}
	std::cout << "\n";
}

std::vector<std::shared_ptr<Monkey>> readInMonkeys(std::istream& in, long long boredomFactor) {
	std::vector<std::shared_ptr<Monkey>> monkeys;
	long long commonDivider = boredomFactor;
	std::string line;
	while (in >> line) {
		size_t monkey_num;
		in >> monkey_num;
		ensureSize(monkeys, monkey_num + 1, boredomFactor);
		skipLine(in);

		std::getline(in, line);
		std::stringstream sitems(line.substr(18));
		long long itemVal;
		while (sitems >> itemVal) {
			monkeys[monkey_num]->receiveItem(itemVal);
			sitems.ignore();
		}

		std::getline(in, line);
		std::stringstream soperation(line.substr(19));
		std::function<long long(long long)> operation = parseOperation(soperation);
		monkeys[monkey_num]->setOperation(operation);

		std::getline(in, line);
		long long divider = std::stoi(line.substr(21));
		commonDivider *= divider;
		monkeys[monkey_num]->setDivider(divider);

		std::getline(in, line);
		size_t trueTarget = std::stoul(line.substr(29));
		ensureSize(monkeys, trueTarget + 1, boredomFactor);
		monkeys[monkey_num]->setTrueTarget(monkeys[trueTarget]);

		std::getline(in, line);
		size_t falseTarget = std::stoul(line.substr(30));
		ensureSize(monkeys, falseTarget + 1, boredomFactor);
		monkeys[monkey_num]->setFalseTarget(monkeys[falseTarget]);
	}
	for (auto& monkey : monkeys) {
		monkey->setCommonDivider(commonDivider);
	}
	return monkeys;
}

void printInspectedItems(const std::vector<std::shared_ptr<Monkey>>& monkeys) {
	for (size_t i = 0; i < monkeys.size(); i++) {
		std::cout << "Monkey " << i << ": " << monkeys[i]->getItemsInspected() << "\n";
	}
}

int f1(std::istream& in) {
	std::vector<std::shared_ptr<Monkey>> monkeys = readInMonkeys(in, 3);

	for (int i = 0; i < 20; i++) {
		for (size_t j = 0; j < monkeys.size(); j++) {
			monkeys[j]->takeTurn();
		}
	}

	auto monkeyComparator = [](const std::shared_ptr<Monkey>& m1, const std::shared_ptr<Monkey>& m2) {
		return m1->getItemsInspected() > m2->getItemsInspected();
	};
	std::nth_element(monkeys.begin(), monkeys.begin() + 1, monkeys.end(), monkeyComparator);

	long long monkeyBusiness = monkeys[0]->getItemsInspected() * monkeys[1]->getItemsInspected();
	std::cout << monkeyBusiness << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::vector<std::shared_ptr<Monkey>> monkeys = readInMonkeys(in, 1);

	for (int i = 0; i < 10000; i++) {
		for (size_t j = 0; j < monkeys.size(); j++) {
			monkeys[j]->takeTurn();
		}
	}

	auto monkeyComparator = [](const std::shared_ptr<Monkey>& m1, const std::shared_ptr<Monkey>& m2) {
		return m1->getItemsInspected() > m2->getItemsInspected();
	};
	std::nth_element(monkeys.begin(), monkeys.begin() + 1, monkeys.end(), monkeyComparator);

	long long monkeyBusiness = monkeys[0]->getItemsInspected() * monkeys[1]->getItemsInspected();
	std::cout << monkeyBusiness << "\n";

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