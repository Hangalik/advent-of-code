#include <iostream>
#include <cstring>

constexpr int BUFFER_SIZE = 16;

bool containsDouble(const char * str, size_t len) {
	for (unsigned int i = 0u; i < len - 1; i++) {
		if (str[i] == str[i + 1]) {
			if (i + 2 >= len || str[i] != str[i + 2]) {
				return true;
			} else {
				char tmp = str[i];
				i += 2;
				while (i + 1 < len && tmp == str[i + 1]) {
					i++;
				}
			}
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
		if (increasing(buffer, len) && containsDouble(buffer, len)) {
			count++;
		}
	}

	std::cout << count << " meet all the criteria." << std::endl;

	return 0;
}
