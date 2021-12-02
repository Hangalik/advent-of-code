#include <fstream>
#include <iostream>
#include <string>

#include "input_selector.h"

int f(std::istream& in) {
	std::string direction;
	int depth = 0, horizont = 0;
	while (in >> direction) {
		int value;
		in >> value;
		if (direction == "forward") {
			horizont += value;
		} else if (direction == "up") {
			depth -= value;
		} else if (direction == "down") {
			depth += value;
		}
	}

	std::cout << "depth * horizont (" << depth << " * " << horizont << ") = " << depth * horizont << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f);

}