#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>

#include "input_selector.h"

enum class Operations {
	ADD, MUL
};

std::pair<Operations, bool> getNextOperation(Operations operation) {
	if (operation == Operations::ADD) {
		return {Operations::MUL, false};
	} else {
		return {Operations::ADD, true};
	}
}

class OperationMixer {
public:
	explicit OperationMixer(size_t n) : elements(n, Operations::ADD) {}

	Operations get(size_t n) const {
		return elements[n];
	}

	bool nextOption() {
		size_t n = 0;
		while (n < elements.size()) {
			auto [nextOp, overflow] = getNextOperation(elements[n]);
			elements[n] = nextOp;
			if (!overflow) {
				break;
			}
			n++;
		}
		return n != elements.size();
	}

private:
	std::vector<Operations> elements;
};

bool isPossible(long long goal, const std::vector<long long>& rest) {
	if (rest.empty()) {
		return false;
	}
	OperationMixer operationMixer(rest.size() - 1);
	do {
		long long result = rest.front();
		for (size_t j = 1uz; j < rest.size(); j++) {
			Operations op = operationMixer.get(j - 1);
			if (op == Operations::ADD) {
				result += rest[j];
			} else if (op == Operations::MUL) {
				result *= rest[j];
			}
			if (result > goal) {
				break;
			}
		}
		if (result == goal) {
			return true;
		}
	} while (operationMixer.nextOption());

	return false;
}

int f1(std::istream& in) {
	long long sum = 0;
	std::string line;
	while (std::getline(in, line)) {
		std::istringstream ss(line);
		long long goal;
		ss >> goal;
		ss.ignore(1);
		std::vector<long long> rest;
		long long other;
		while (ss >> other) {
			rest.push_back(other);
		}
		if (isPossible(goal, rest)) {
			sum += goal;
		}
	}

	std::cout << sum << "\n";

	return 0;
}


enum class Operations2 {
	ADD, MUL, CONCAT
};

std::pair<Operations2, bool> getNextOperation2(Operations2 operation) {
	if (operation == Operations2::ADD) {
		return {Operations2::MUL, false};
	} else if (operation == Operations2::MUL) {
		return {Operations2::CONCAT, false};
	} else {
		return {Operations2::ADD, true};
	}
}

class OperationMixer2 {
public:
	explicit OperationMixer2(size_t n) : elements(n, Operations2::ADD) {}

	Operations2 get(size_t n) const {
		return elements[n];
	}

	bool nextOption() {
		size_t n = 0;
		while (n < elements.size()) {
			auto [nextOp, overflow] = getNextOperation2(elements[n]);
			elements[n] = nextOp;
			if (!overflow) {
				break;
			}
			n++;
		}
		return n != elements.size();
	}

private:
	std::vector<Operations2> elements;
};

bool isPossible2(long long goal, const std::vector<long long>& rest) {
	if (rest.empty()) {
		return false;
	}
	OperationMixer2 operationMixer(rest.size() - 1);
	do {
		long long result = rest.front();
		for (size_t j = 1uz; j < rest.size(); j++) {
			Operations2 op = operationMixer.get(j - 1);
			if (op == Operations2::ADD) {
				result += rest[j];
			} else if (op == Operations2::MUL) {
				result *= rest[j];
			} else if (op == Operations2::CONCAT) {
				long long asd = std::stoll(std::to_string(result) + std::to_string(rest[j]));
				result = asd;
			}
			if (result > goal) {
				break;
			}
		}
		if (result == goal) {
			return true;
		}
	} while (operationMixer.nextOption());

	return false;
}

int f2(std::istream& in) {
	long long sum = 0;
	std::string line;
	while (std::getline(in, line)) {
		std::istringstream ss(line);
		long long goal;
		ss >> goal;
		ss.ignore(1);
		std::vector<long long> rest;
		long long other;
		while (ss >> other) {
			rest.push_back(other);
		}
		if (isPossible2(goal, rest)) {
			sum += goal;
		}
	}

	std::cout << sum << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}