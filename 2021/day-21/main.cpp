#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

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

const std::unordered_map<int, int> stepMultipliers{
	{3, 1},
	{4, 3},
	{5, 6},
	{6, 7},
	{7, 6},
	{8, 3},
	{9, 1},
};

class ParallelPlayer {
public:
	explicit ParallelPlayer(int startPosition) : position(startPosition - 1), points(0), universes(1) {}

	std::vector<ParallelPlayer> step() const {
		std::vector<ParallelPlayer> parallels;
		for (const auto& multi : stepMultipliers) {
			int newPosition = (position + multi.first) % maxPosition;
			int newPoints = points + newPosition + 1;
			long long newUniverses = universes * multi.second;
			ParallelPlayer newPlayer{newPosition, newPoints, newUniverses};
			parallels.push_back(newPlayer);
		}
		return parallels;
	}

	int getPoints() const {
		return points;
	}

	long long getUniverses() const {
		return universes;
	}

private:
	static constexpr int maxPosition = 10;

	ParallelPlayer(int position, int points, long long universes) :
		position(position),
		points(points),
		universes(universes)
	{}

	int position;
	int points;
	long long universes;
};

int f2(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');
	int start1 = line.back() - '0';
	std::getline(in, line, '\n');
	int start2 = line.back() - '0';

	std::vector<ParallelPlayer> player1Parallels{ParallelPlayer(start1)};
	std::vector<ParallelPlayer> player2Parallels{ParallelPlayer(start2)};
	std::vector<ParallelPlayer>* current = &player1Parallels;
	long long currentWins = 0;
	std::vector<ParallelPlayer>* other = &player2Parallels;
	long long otherWins = 0;

	while (true) {
		std::vector<ParallelPlayer> newPlayers;
		long long otherUniverseCount = 0;
		for (const ParallelPlayer& pp : *other) {
			otherUniverseCount += pp.getUniverses();
		}
		for (const ParallelPlayer& pp : *current) {
			std::vector<ParallelPlayer> parallels = pp.step();
			for (const ParallelPlayer& newPlayer : parallels) {
				if (newPlayer.getPoints() >= 21) {
					currentWins += newPlayer.getUniverses() * otherUniverseCount;
				} else {
					newPlayers.push_back(newPlayer);
				}
			}
		}
		*current = std::move(newPlayers);
		if (current->empty()) {
			break;
		}
		std::swap(current, other);
		std::swap(currentWins, otherWins);
	}

	std::cout << std::max(currentWins, otherWins) << std::endl;

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
