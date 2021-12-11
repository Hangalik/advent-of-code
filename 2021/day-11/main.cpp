#include <iostream>
#include <string>
#include <unordered_set>
#include <deque>

#include "input_selector.h"

class Octopus {
public:
	explicit Octopus(int energy) : energy(energy) {}

	void addNeighbour(Octopus* newNeighbour) {
		if (neighbours.size() >= 8u) {
			throw std::length_error("octopuses cannot have more than 8 neighbours");
		}
		neighbours.insert(newNeighbour);
	}

	const std::unordered_set<Octopus*> getNeighbours() const {
		return neighbours;
	}

	bool increaseEnergy() {
		energy++;
		if (energy > 9) {
			energy = 0;
			return true;
		}
		return false;
	}

	int getEnergy() const {
		return energy;
	}

private:
	std::unordered_set<Octopus*> neighbours;
	int energy;
};

void linkOctopuses(Octopus& lhs, Octopus& rhs) {
	lhs.addNeighbour(&rhs);
	rhs.addNeighbour(&lhs);
}

void printOctopuses(const std::deque<std::deque<Octopus>>& octopuses) {
	for (const std::deque<Octopus>& octopusLine : octopuses) {
		for (const Octopus& octopus : octopusLine) {
			std::cout << octopus.getEnergy();
		}
		std::cout << std::endl;
	}
}

int stepOctopuses(std::deque<std::deque<Octopus>>& octopuses) {
	std::unordered_multiset<Octopus*> toIncreaseOctopuses;
	for (std::deque<Octopus>& octopusLine : octopuses) {
		for (Octopus& octopus : octopusLine) {
			toIncreaseOctopuses.insert(&octopus);
		}
	}

	int flashCount = 0;
	std::unordered_set<Octopus*> alreadyFlashedOctopuses;
	while (!toIncreaseOctopuses.empty()) {
		auto first = toIncreaseOctopuses.begin();
		Octopus* octopus = *first;
		toIncreaseOctopuses.erase(first);
		if (octopus->increaseEnergy()) {
			flashCount++;
			alreadyFlashedOctopuses.insert(octopus);
			toIncreaseOctopuses.erase(octopus);
			for (Octopus* neighbour : octopus->getNeighbours()) {
				if (!alreadyFlashedOctopuses.contains(neighbour)) {
					toIncreaseOctopuses.insert(neighbour);
				}
			}
		}
	}
	return flashCount;
}

int f1(std::istream& in) {
	std::string line;
	std::deque<std::deque<Octopus>> octopuses;
	while (in >> line) {
		std::deque<Octopus> octopusLine;
		for (size_t i = 0; i < line.size(); i++) {
			int energy = std::stoi(line.substr(i, 1));
			octopusLine.emplace_back(energy);

			bool hasPreviousInLine = i > 0u;
			if (hasPreviousInLine) {
				linkOctopuses(octopusLine[i], octopusLine[i - 1]);
			}
			bool hasPreviousLine = !octopuses.empty();
			if (hasPreviousLine) {
				linkOctopuses(octopusLine[i], octopuses.back()[i]);
			}
			bool previousLineHasBefore = hasPreviousInLine && hasPreviousLine;
			if (previousLineHasBefore) {
				linkOctopuses(octopusLine[i], octopuses.back()[i - 1]);
			}
			bool previousLineHasAfter = hasPreviousLine && octopuses.back().size() > i + 1;
			if (previousLineHasAfter) {
				linkOctopuses(octopusLine[i], octopuses.back()[i + 1]);
			}
		}
		octopuses.push_back(std::move(octopusLine));
	}

	int sumOfFlashes = 0;
	for (int i = 0; i < 100; i++) {
		sumOfFlashes += stepOctopuses(octopuses);
	}

	std::cout << sumOfFlashes << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::string line;
	std::deque<std::deque<Octopus>> octopuses;
	while (in >> line) {
		std::deque<Octopus> octopusLine;
		for (size_t i = 0; i < line.size(); i++) {
			int energy = std::stoi(line.substr(i, 1));
			octopusLine.emplace_back(energy);

			bool hasPreviousInLine = i > 0u;
			if (hasPreviousInLine) {
				linkOctopuses(octopusLine[i], octopusLine[i - 1]);
			}
			bool hasPreviousLine = !octopuses.empty();
			if (hasPreviousLine) {
				linkOctopuses(octopusLine[i], octopuses.back()[i]);
			}
			bool previousLineHasBefore = hasPreviousInLine && hasPreviousLine;
			if (previousLineHasBefore) {
				linkOctopuses(octopusLine[i], octopuses.back()[i - 1]);
			}
			bool previousLineHasAfter = hasPreviousLine && octopuses.back().size() > i + 1;
			if (previousLineHasAfter) {
				linkOctopuses(octopusLine[i], octopuses.back()[i + 1]);
			}
		}
		octopuses.push_back(std::move(octopusLine));
	}

	int step = 0;
	while (true) {
		step++;
		int flashCount = stepOctopuses(octopuses);
		if (flashCount == 100) {
			break;
		}
	}

	std::cout << step << std::endl;

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
