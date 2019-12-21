#include <iostream>
#include <cstring>

constexpr int BUFFER_SIZE = 16;

bool containsDouble(const char * str, size_t len) {
	for (unsigned int i = 1u; i < len; i++) {
		if (str[i] == str[i - 1]) {
			return true;
		}
	}
	return false;
}

bool increasing(const char * str, size_t len) {
	for (unsigned int i = 1u; i < len; i++) {
		if (str[i] < str[i - 1]) {
			return false;
		}
	}
	return true;
}

int main() {

	char lowerEdgeHolder[BUFFER_SIZE];
	std::cin.getline(lowerEdgeHolder, BUFFER_SIZE, '-');

	char higherEdgeHolder[BUFFER_SIZE];
	std::cin.getline(higherEdgeHolder, BUFFER_SIZE, '-');

	int lowererEdge{ std::stoi(lowerEdgeHolder) };
	int higherEdge{ std::stoi(higherEdgeHolder) };

	int count{ 0 };

	for (int i = lowererEdge; i <= higherEdge; i++) {
		char buffer[BUFFER_SIZE];
		int len = sprintf(buffer, "%d", i);
		if (containsDouble(buffer, len) && increasing(buffer, len)) {
			count++;
		}
	}

	std::cout << count << " satisfies these criterias." << std::endl;
	return 0;
}
