#include <iostream>
#include <string>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <limits>

#include "input_selector.h"

class Chiton {
public:
	explicit Chiton(int value) : value(value) {}

	void addNeighbour(Chiton* newNeighbour) {
		if (neighbours.size() > 4) {
			throw std::length_error("chitons cannot have more than 4 neighbours");
		}
		neighbours.insert(newNeighbour);
	}

	int getValue() const {
		return value;
	}

	const std::unordered_set<Chiton*>& getNeighbours() const {
		return neighbours;
	}

private:
	int value;
	std::unordered_set<Chiton*> neighbours;
};

void linkChitons(Chiton& lhs, Chiton& rhs) {
	lhs.addNeighbour(&rhs);
	rhs.addNeighbour(&lhs);
}

int f1(std::istream& in) {
	std::deque<std::deque<Chiton>> chitons;
	std::unordered_set<Chiton*> unvisitedChitons;
	std::string line;
	while (std::getline(in, line, '\n')) {
		std::deque<Chiton> chitonLine;
		for (size_t i = 0; i < line.size(); i++) {
			int value = line[i] - '0';
			chitonLine.emplace_back(value);
			unvisitedChitons.insert(&chitonLine.back());
			if (i > 0u) {
				linkChitons(chitonLine[i], chitonLine[i - 1]);
			}
			if (!chitons.empty()) {
				linkChitons(chitons.back()[i], chitonLine[i]);
			}
		}
		chitons.push_back(std::move(chitonLine));
	}

	constexpr int inf = std::numeric_limits<int>::max();
	std::unordered_map<Chiton*, int> shortestPaths;
	for (Chiton* ch : unvisitedChitons) {
		shortestPaths.emplace(ch, inf);
	}
	shortestPaths[&chitons[0][0]] = 0;
	while (!unvisitedChitons.empty()) {
		Chiton* minChiton = *unvisitedChitons.begin();
		int shortestPath = shortestPaths.at(minChiton);
		for (Chiton* ch : unvisitedChitons) {
			int challenger = shortestPaths.at(ch);
			if (challenger < shortestPath) {
				shortestPath = challenger;
				minChiton = ch;
			}
		}
		unvisitedChitons.erase(minChiton);
		for (Chiton* neighbour : minChiton->getNeighbours()) {
			shortestPaths[neighbour] = std::min(shortestPaths[neighbour], shortestPath + neighbour->getValue());
		}
	}

	std::cout << shortestPaths.at(&chitons.back().back()) << std::endl;

	return 0;
}

char getIncreasedVal(char original, char increase) {
	char inc = original + increase;
	if (inc > '9') {
		return inc - 9;
	}
	return inc;
}

int getIncreasedVal(int original, int increase) {
	int inc = original + increase;
	if (inc > 9) {
		return inc - 9;
	}
	return inc;
}

std::string extendLine(const std::string& line) {
	std::string extended = line;
	for (char i = 1; i < 5; i++) {
		for (char c : line) {
			extended.push_back(getIncreasedVal(c, i));
		}
	}
	return extended;
}

int f2(std::istream& in) {
	std::deque<std::deque<Chiton>> chitons;
	std::unordered_set<Chiton*> unvisitedChitons;
	std::string line;
	while (std::getline(in, line, '\n')) {
		std::string extendedLine = extendLine(line);
		std::deque<Chiton> chitonLine;
		for (size_t i = 0; i < extendedLine.size(); i++) {
			int value = extendedLine[i] - '0';
			chitonLine.emplace_back(value);
			unvisitedChitons.insert(&chitonLine.back());
		}
		chitons.push_back(std::move(chitonLine));
	}
	size_t initialChitonSize = chitons.size();
	for (int i = 1; i < 5; i++) {
		for (size_t j = 0; j < initialChitonSize; j++) {
			std::deque<Chiton> additionalChitons;
			for (const Chiton& ch : chitons[j]) {
				additionalChitons.emplace_back(getIncreasedVal(ch.getValue(), i));
				unvisitedChitons.insert(&additionalChitons.back());
			}
			chitons.push_back(std::move(additionalChitons));
		}
	}
	for (size_t i = 0; i < chitons.size(); i++) {
		for (size_t j = 0; j < chitons[i].size(); j++) {
			if (i > 0u) {
				linkChitons(chitons[i][j], chitons[i - 1][j]);
			}
			if (j > 0u) {
				linkChitons(chitons[i][j], chitons[i][j - 1]);
			}
		}
	}

	constexpr int inf = std::numeric_limits<int>::max();
	std::unordered_map<Chiton*, int> shortestPaths;
	for (Chiton* ch : unvisitedChitons) {
		shortestPaths.emplace(ch, inf);
	}
	Chiton* startChiton = &chitons[0][0];
	Chiton* endChiton = &chitons.back().back();
	shortestPaths[startChiton] = 0;
	std::unordered_set<Chiton*> activeChitons;
	activeChitons.insert(startChiton);
	while (!unvisitedChitons.empty()) {
		Chiton* minChiton = *activeChitons.begin();
		int shortestPath = shortestPaths.at(minChiton);
		for (Chiton* ch : activeChitons) {
			int challenger = shortestPaths.at(ch);
			if (challenger < shortestPath) {
				shortestPath = challenger;
				minChiton = ch;
			}
		}
		if (minChiton == endChiton) {
			break;
		}
		unvisitedChitons.erase(minChiton);
		activeChitons.erase(minChiton);
		for (Chiton* neighbour : minChiton->getNeighbours()) {
			shortestPaths[neighbour] = std::min(shortestPaths[neighbour], shortestPath + neighbour->getValue());
			if (unvisitedChitons.contains(neighbour)) {
				activeChitons.insert(neighbour);
			}
		}
	}

	std::cout << shortestPaths.at(endChiton) << std::endl;

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
