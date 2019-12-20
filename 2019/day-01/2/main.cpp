#include <iostream>
#include <algorithm>

int calculateFuelNeeded(int mass) {
	int rawFuel = (mass / 3) - 2;
	return std::max(0, rawFuel);
}

int main() {
	
	int fuelSum{ 0 };
	int next;
	while (std::cin >> next) {
		while (next > 0) {
			int temp = calculateFuelNeeded(next);
			next = temp;
			fuelSum += temp;
		}
	}
	std::cout << "Fuel needed: " << fuelSum << std::endl;
	return 0;
}
