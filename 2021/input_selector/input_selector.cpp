#include "input_selector.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

int runWithProperInput(int argc, const char* argv[], std::function<int(std::istream&)> func) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
		return 1;
	}

	const char* filename = argv[1];
	if (std::strcmp(filename, "-") == 0) {
		return func(std::cin);
	} else {
		if (!std::filesystem::exists(filename)) {
			std::cerr << "The file " << filename << "  doesn't exist" << std::endl;
			return 2;
		}
		std::ifstream file(filename);
		return func(file);
	}

	return 0;
}