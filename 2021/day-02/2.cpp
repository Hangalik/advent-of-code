#include <fstream>
#include <iostream>
#include <string>

#include "input_selector.h"

int f(std::istream& in) {
	std::string direction;
	int depth = 0, horizont = 0, aim = 0;
	while (in >> direction) {
		int value;
		in >> value;
		if (direction == "forward") {
			horizont += value;
			depth += aim * value;
		} else if (direction == "up") {
			aim -= value;
		} else if (direction == "down") {
			aim += value;
		}
	}

	std::cout << "depth * horizont (" << depth << " * " << horizont << ") = " << depth * horizont << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f);

}