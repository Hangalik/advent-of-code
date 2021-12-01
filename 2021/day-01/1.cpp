#include <fstream>
#include <iostream>

#include "input_selector.h"

int f(std::istream& in) {
	int previous, current;
	int increaseCount = 0;
	in >> previous;
	while (in >> current) {
		if (previous < current) {
			increaseCount++;
		}
		previous = current;
	}
	
	std::cout << "Increase count: " << increaseCount << std::endl;
	
	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f);
	
}