#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <tuple>

#include "input_selector.h"

std::vector<std::stack<char, std::vector<char>>> readStacks(std::istream& in) {
	std::vector<std::vector<char>> stacks;
	while (true) {
		std::string line;
		std::getline(in, line);
		size_t stack_number = line.length() / 4 + 1;
		if (stacks.size() < stack_number) {
			for (size_t i = stacks.size(); i < stack_number; i++) {
				stacks.emplace_back();
			}
		}
		if (std::isdigit(line[1])) {
			std::getline(in, line);
			break;
		}
		for (size_t i = 0; i < stack_number; i++) {
			char c = line[1 + 4 * i];
			if (!std::isspace(c)) {
				stacks[i].push_back(c);
			}
		}
	}
	std::vector<std::stack<char, std::vector<char>>> result;
	for (const auto& s : stacks) {
		result.emplace_back(std::vector<char>(s.rbegin(), s.rend()));
	}
	return result;
}

void printStacks(const std::vector<std::stack<char, std::vector<char>>>& stacks) {
	int i = 1;
	for (auto stack : stacks) {
		std::cout << i << " ";
		while (!stack.empty()) {
			std::cout << stack.top();
			stack.pop();
		}
		i++;
		std::cout << "\n";
	}
}

std::tuple<size_t, size_t, size_t> readInstruction(const std::string& line) {
	std::string ignore;
	size_t i1, i2, i3;

	std::istringstream in(line);
	in >> ignore >> i1 >> ignore >> i2 >> ignore >> i3;

	return {i1, i2, i3};
}

int f1(std::istream& in) {

	std::vector<std::stack<char, std::vector<char>>> stacks = readStacks(in);

	std::string line;
	while (std::getline(in, line)) {
		auto [num, from, to] = readInstruction(line);
		for (size_t i = 0 ; i < num; i++) {
			stacks[to - 1].push(stacks[from - 1].top());
			stacks[from - 1].pop();
		}
	}

	for (const auto& stack : stacks) {
		std::cout << stack.top();
	}
	std::cout << "\n";

	return 0;
}

int f2(std::istream& in) {

	std::vector<std::stack<char, std::vector<char>>> stacks = readStacks(in);

	std::string line;
	while (std::getline(in, line)) {
		std::stack<char, std::vector<char>> tempStack;
		auto [num, from, to] = readInstruction(line);
		for (size_t i = 0 ; i < num; i++) {
			tempStack.push(stacks[from - 1].top());
			stacks[from - 1].pop();
		}
		for (size_t i = 0 ; i < num; i++) {
			stacks[to - 1].push(tempStack.top());
			tempStack.pop();
		}
	}

	for (const auto& stack : stacks) {
		std::cout << stack.top();
	}
	std::cout << "\n";

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