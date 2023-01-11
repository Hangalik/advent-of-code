#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <utility>

#include <boost/functional/hash.hpp>

#include "input_selector.h"

class Valve {
public:
	Valve(std::string name, int rate, std::unordered_set<std::string> neighbours) :
			name(std::move(name)),
			rate(rate),
			neighbours(std::move(neighbours))
	{}

	std::string name;
	int rate;
	std::unordered_set<std::string> neighbours;
};

class ValveSystem {
public:

	void addValve(const Valve& valve) {
		auto result = valves.try_emplace(valve.name, valve.rate, valve.neighbours);
		if (!result.second) { throw std::logic_error("ValveSystem::addValve couldn't insert into valves"); }
	}

	void simplify() {
	}

	int getAllRate(const std::set<std::string>& openValves) const {
		int allRates = 0;
		for (const std::string& openValve : openValves) {
			auto found = valves.find(openValve);
			if (found != valves.end()) {
				allRates += found->second.first;
			}
		}
		return allRates;
	}

	int getRate(const std::string& valve) const {
		auto found = valves.find(valve);
		if (found == valves.end()) {
			throw std::logic_error("ValveSystem::getNeighbours no souch valve");
		}
		return found->second.first;
	}

	std::unordered_set<std::string> getNeighbours(const std::string& valve) const {
		auto found = valves.find(valve);
		if (found == valves.end()) {
			throw std::logic_error("ValveSystem::getNeighbours no souch valve");
		}
		return found->second.second;
	}

private:
	std::unordered_map<std::string, std::pair<int, std::unordered_set<std::string>>> valves;
};

ValveSystem readInValveSystem(std::istream& in) {
	ValveSystem vs;

	std::string line;
	while (std::getline(in, line)) {
		std::stringstream sline(line);
		sline.ignore(6);
		std::string valveName;
		sline >> valveName;
		sline.ignore(15);
		int rate;
		sline >> rate;
		sline.ignore(2);

		std::string ignore;
		sline >> ignore >> ignore >> ignore >> ignore;
		sline.ignore(1);

		std::unordered_set<std::string> neighbours;
		std::string neighbour;
		while (sline >> neighbour) {
			neighbour.resize(2);
			neighbours.insert(std::move(neighbour));
		}
		vs.addValve(Valve{valveName, rate, neighbours});
	}

	return vs;
}

struct SituationState {
	std::string position;
	std::set<std::string> openedValves;

	bool operator==(const SituationState& other) const = default;
};

struct StateHasher {
	size_t operator()(const SituationState& ss) const {
		size_t seed = 0;
		boost::hash_combine(seed, ss.position);
		for (const std::string& openValve : ss.openedValves) {
			boost::hash_combine(seed, openValve);
		}
		return seed;
	}
};

void insertIfMax(std::unordered_map<SituationState, int, StateHasher>& stateSpace, const SituationState& ss, int pressureReleased) {
	if (stateSpace[ss] < pressureReleased) {
		stateSpace[ss] = pressureReleased;
	}
}

int mostPressureReleased(const ValveSystem& vs, const std::string& start, int time) {
	std::unordered_map<SituationState, int, StateHasher> stateSpace;
	stateSpace.try_emplace(SituationState{start, std::set<std::string>{}}, 0);

	for (int i = 0; i < time; i++) {
		std::cout << i << "\n";
		std::unordered_map<SituationState, int, StateHasher> newStateSpace;
		for (const auto& [ss, releasedPressure] : stateSpace) {
			int newReleasedPressure = releasedPressure + vs.getAllRate(ss.openedValves);
			if (!ss.openedValves.contains(ss.position) && vs.getRate(ss.position) != 0) {
				std::set<std::string> newOpenedValves = ss.openedValves;
				newOpenedValves.insert(ss.position);
				SituationState nextState{ss.position, newOpenedValves};
				insertIfMax(newStateSpace, nextState, newReleasedPressure);
			}
			for (const std::string& neighbour : vs.getNeighbours(ss.position)) {
				SituationState nextState{neighbour, ss.openedValves};
				insertIfMax(newStateSpace, nextState, newReleasedPressure);
			}
		}
		stateSpace = std::move(newStateSpace);
	}

	int maxPressuse = 0;
	for (const auto& [ign, pressureReleased] : stateSpace) {
		if (pressureReleased > maxPressuse) {
			maxPressuse = pressureReleased;
		}
	}

	return maxPressuse;
}

static const std::string startingValve = "AA";
static constexpr int allTime = 30;

int f1(std::istream& in) {
	ValveSystem vs = readInValveSystem(in);
	int res = mostPressureReleased(vs, startingValve, allTime);

	std::cout << res << "\n";

	return 0;
}

int f2(std::istream& in) {

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