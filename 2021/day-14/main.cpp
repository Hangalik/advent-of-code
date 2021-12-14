#include <iostream>
#include <string>

#include "input_selector.h"

int f1(std::istream& in) {
	std::string templatePolymer;
	in >> templatePolymer;
	in.ignore(10, '\n');



	return 0;
}

int f2(std::istream& in) {

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
