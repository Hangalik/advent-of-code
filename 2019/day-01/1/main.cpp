#include <iostream>

int main() {
	
	int fuelSum{ 0 };
	int next;
	while (std::cin >> next) {
		fuelSum += (next / 3) - 2;
	}
	std::cout << "Fuel needed: " << fuelSum << std::endl;
	return 0;
}
