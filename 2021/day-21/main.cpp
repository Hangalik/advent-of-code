#include <iostream>
#include <string>

#include "input_selector.h"

class DeterministicDie {
public:
	DeterministicDie() : rollTimes(0), nextValue(6) {}

	int getNext() {
		int next = nextValue;
		nextValue--;
		if (nextValue < 0) {
			nextValue = resetValue;
		}
		rollTimes += 3;
		return next;
	}

	int getRollTimes() const {
		return rollTimes;
	}

private:
	static constexpr int resetValue = 9;

	int rollTimes;
	int nextValue;
};

class Player {
public:
	Player(int startPosition) : position(startPosition - 1), points(0) {}

	void stepForward(int steps) {
		position += steps;
		position %= maxPosition;
		points += position + 1;
	}

	bool isPointsAtLeast(int goal) const {
		return points >= goal;
	}

	int getPoints() const {
		return points;
	}

private:
	static constexpr int maxPosition = 10;

	int position;
	int points;
};

int f1(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');
	int start1 = line.back() - '0';
	std::getline(in, line, '\n');
	int start2 = line.back() - '0';

	DeterministicDie die;
	Player player1(start1);
	Player player2(start2);

	Player* current = &player1;
	Player* other = &player2;
	while (true) {
		current->stepForward(die.getNext());
		if (current->isPointsAtLeast(1000)) {
			break;
		}
		std::swap(current, other);
	}
	int loserPoints = other->getPoints();
	int rollTimes = die.getRollTimes();
	std::cout << loserPoints << " * " << rollTimes << " = " << loserPoints * rollTimes << std::endl;

	return 0;
}

int f2(std::istream& ) {

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::logic_error& le) {
		std::cerr << le.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
	return 1;
}
