#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "input_selector.h"

void printVector(const std::vector<int> ints) {
	for (int i : ints) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}

std::vector<int> getRandomNumbers(std::istream& in) {
	std::string randomStrings;
	std::getline(in, randomStrings);
	std::vector<int> randomNumbers;
	std::string randomNumber;
	for (std::istringstream randomStream(std::move(randomStrings)); std::getline(randomStream, randomNumber, ','); ) {
		randomNumbers.push_back(std::stoi(randomNumber));
	}
	return randomNumbers;
}

class Bingo {
public:
	Bingo() : bingo(), marked{false} {}

	int& value(int row, int column) {
		return bingo[row * 5 + column];
	}

	const int& value(int row, int column) const {
		return bingo[row * 5 + column];
	}

	int sumOfAllUnmarked() const {
		int sum = 0;
		for (int i = 0; i < 25; i++) {
			if (!marked[i]) {
				sum += bingo[i];
			}
		}
		return sum;
	}

	void mark(int num) {
		for (int i = 0; i < 25; i++) {
			if (bingo[i] == num) {
				marked[i] = true;
			}
		}
	}

	bool isBingo() const {
		for (int i = 0; i < 5; i++) {
			if (isBingoInColumn(i) || isBingoInRow(i)) {
				return true;
			}
		}
		return false;
	}

private:
	bool isBingoInRow(int row) const {
		for (int column = 0; column < 5; column++) {
			if (!marked[row * 5 + column]) {
				return false;
			}
		}
		return true;
	}

	bool isBingoInColumn(int column) const {
		for (int row = 0; row < 5; row++) {
			if (!marked[row * 5 + column]) {
				return false;
			}
		}
		return true;
	}

	int bingo[25];
	bool marked[25];
};

Bingo readBingo(std::istream& in) {
	Bingo b;
	for (int row = 0; row < 5; row++) {
		for (int column = 0; column < 5; column++) {
			in >> b.value(row, column);
		}
	}
	return b;
}

void printBingo(const Bingo& b) {
	for (int row = 0; row < 5; row++) {
		for (int column = 0; column < 5; column++) {
			std::cout << std::setw(3) << b.value(row, column);
		}
		std::cout << std::endl;
	}
}

int f1(std::istream& in) {

	std::vector<int> randomNumbers = getRandomNumbers(in);

	std::vector<Bingo> bingos;
	while (in.good()) {
		bingos.push_back(readBingo(in));
	}

	for (int random : randomNumbers) {
		for (Bingo& b : bingos) {
			b.mark(random);
			if (b.isBingo()) {
				int soaum = b.sumOfAllUnmarked();
				std::cout << soaum << " * " << random << " = " << soaum * random << std::endl;
				return 0;
			}
		}
	}

	return 0;
}

int f2(std::istream& in) {
	std::vector<int> randomNumbers = getRandomNumbers(in);

	std::vector<Bingo> bingos;
	while (in.good()) {
		bingos.push_back(readBingo(in));
	}

	for (int random : randomNumbers) {
		for (Bingo& b : bingos) {
			b.mark(random);
		}
		if (bingos.size() > 1) {
			std::erase_if(bingos, [](const Bingo& b) { return b.isBingo(); });
		} else if (bingos[0].isBingo()) {
			int soaum = bingos[0].sumOfAllUnmarked();
			std::cout << soaum << " * " << random << " = " << soaum * random << std::endl;
			return 0;
		}
	}

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);

}