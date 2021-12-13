#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>

#include "input_selector.h"

bool isAllUpper(const std::string& str) {
	return std::all_of(str.begin(), str.end(), [](char c) {
		return std::isupper(static_cast<unsigned char>(c));
	});
}

class Cave;

void printRoute(const std::vector<const Cave*>& route);

class Cave {
public:
	explicit Cave(std::string name) :
		name(std::move(name)),
		big(isAllUpper(this->name))
	{}

	void addNeighbour(Cave* newNeighbour) {
		neighbours.insert(newNeighbour);
	}

	void removeNeighbour(Cave* neighbour) {
		neighbours.erase(neighbour);
	}

	const std::string& getName() const {
		return name;
	}

	bool isBig() const {
		return big;
	}

	const std::unordered_set<Cave*>& getNeighbours() const {
		return neighbours;
	}

	int getPossibleRoutesTo(std::string destination) const {
		std::vector<const Cave*> alreadyVisitedCaves;
		return getPossibleRoutesTo(alreadyVisitedCaves, destination);
	}

	int getPossibleRoutesToWithSingleDuplicate(std::string destination) const {
		std::vector<const Cave*> alreadyVisitedCaves;
		return getPossibleRoutesToWithSingleDuplicate(alreadyVisitedCaves, destination, false);
	}

private:
	int getPossibleRoutesTo(std::vector<const Cave*> visitedCaves, const std::string& destination) const {
		if (name == destination) {
			//printRoute(visitedCaves);
			return 1;
		}
		visitedCaves.push_back(this);
		int possibleRoutes = 0;
		for (const Cave* neighbour : neighbours) {
			if (neighbour->isBig() || std::find(visitedCaves.begin(), visitedCaves.end(), neighbour) == visitedCaves.end()) {
				possibleRoutes += neighbour->getPossibleRoutesTo(visitedCaves, destination);
			}
		}
		visitedCaves.pop_back();
		return possibleRoutes;
	}

	int getPossibleRoutesToWithSingleDuplicate(std::vector<const Cave*> visitedCaves, const std::string& destination, bool alreadyHasDuplicate) const {
		if (name == destination) {
			//printRoute(visitedCaves);
			return 1;
		}
		if (!big) {
			bool alreadyVisited = std::find(visitedCaves.begin(), visitedCaves.end(), this) != visitedCaves.end();
			if (alreadyVisited) {
				if (alreadyHasDuplicate || name == "start") {
					return 0;
				}
				alreadyHasDuplicate = true;
			}
		}
		visitedCaves.push_back(this);
		int possibleRoutes = 0;
		for (const Cave* neighbour : neighbours) {
			possibleRoutes += neighbour->getPossibleRoutesToWithSingleDuplicate(visitedCaves, destination, alreadyHasDuplicate);
		}
		visitedCaves.pop_back();
		return possibleRoutes;
	}

	std::string name;
	bool big;

	std::unordered_set<Cave*> neighbours;
};

void printRoute(const std::vector<const Cave*>& route) {
	for (const Cave* cave : route) {
		std::cout << cave->getName() << ",";
	}
	std::cout << "end" << std::endl;
}

void linkCaves(Cave& lhs, Cave& rhs) {
	lhs.addNeighbour(&rhs);
	rhs.addNeighbour(&lhs);
}

int f1(std::istream& in) {
	std::unordered_map<std::string, Cave> caves;
	std::string line;
	while (in >> line) {
		size_t half = line.find('-');
		std::string first = line.substr(0, half);
		std::string second = line.substr(half + 1);

		caves.try_emplace(first, first);
		caves.try_emplace(second, second);

		linkCaves(caves.at(first), caves.at(second));
	}

	int possibleRoutesFromStartToEnd = caves.at("start").getPossibleRoutesTo("end");
	
	std::cout << possibleRoutesFromStartToEnd << std::endl;

	return 0;
}

class SmallCaveDuplicator {
public:
	explicit SmallCaveDuplicator(Cave& smallCave) : duplicateCave(smallCave) {
		for (Cave* cave : duplicateCave.getNeighbours()) {
			cave->addNeighbour(&duplicateCave);
		}
	}

	~SmallCaveDuplicator() {
		for (Cave* cave : duplicateCave.getNeighbours()) {
			cave->removeNeighbour(&duplicateCave);
		}
	}

private:
	Cave duplicateCave;
};

int f2(std::istream& in) {
	std::unordered_map<std::string, Cave> caves;
	std::string line;
	while (in >> line) {
		size_t half = line.find('-');
		std::string first = line.substr(0, half);
		std::string second = line.substr(half + 1);

		caves.try_emplace(first, first);
		caves.try_emplace(second, second);

		linkCaves(caves.at(first), caves.at(second));
	}

/*
	const int possibleRoutesFromStartToEnd = caves.at("start").getPossibleRoutesTo("end");
	int withDuplicateSmallCaveRouteCount = possibleRoutesFromStartToEnd;
	for (auto& [name, cave] : caves) {
		if (name == "start" || name == "end") {
			continue;
		}
		if (!cave.isBig()) {
			SmallCaveDuplicator duplicateSmallCave(cave);
			std::cout << name << " neighbours: " << cave.getNeighbours().size() << std::endl;
			int extraRoutes = caves.at("start").getPossibleRoutesTo("end") - possibleRoutesFromStartToEnd;
			withDuplicateSmallCaveRouteCount += extraRoutes / 2;
			std::cout << name << ": " << extraRoutes << std::endl;
		}
	}
	*/
	int withDuplicateSmallCaveRouteCount = caves.at("start").getPossibleRoutesToWithSingleDuplicate("end");
	
	std::cout << withDuplicateSmallCaveRouteCount << std::endl;

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
