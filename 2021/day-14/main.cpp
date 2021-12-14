#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

#include "input_selector.h"

struct PairHasher {
	size_t operator()(const std::pair<char, char>& v) const {
		return 31 * v.first + v.second;
	}
};

void addCounts(std::unordered_map<char, long long>& newCache, const std::unordered_map<char, long long>& oldCache) {
	for (const auto& cacheEntry : oldCache) {
		newCache[cacheEntry.first] += cacheEntry.second;
	}
}

class PolymerInserter {
public:
	void addInsertionRule(char in1, char in2, char out) {
		insertionRules.insert({{in1, in2}, out});
	}

	char getInsertChar(char in1, char in2) const {
		return insertionRules.at({in1, in2});
	}

	std::unordered_map<char, long long> getPolymerPartCounts(const std::string& templatePolymer, size_t steps) const {
		std::unordered_map<char, long long> polymerPartCounts;
		for (char c : templatePolymer) {
			polymerPartCounts[c]++;
		}
		if (templatePolymer.size() < 2 || steps == 0u) {
			return polymerPartCounts;
		}
		for (size_t i = 1; i < templatePolymer.size(); i++) {
			getPolymerPartCounts(polymerPartCounts, templatePolymer[i - 1], templatePolymer[i], steps);
		}
		return polymerPartCounts;
	}
	
	std::unordered_map<char, long long> getPolymerPartCountsCached(const std::string& templatePolymer, size_t steps) const {
		std::unordered_map<char, long long> polymerPartCounts;
		for (char c : templatePolymer) {
			polymerPartCounts[c]++;
		}
		if (templatePolymer.size() < 2 || steps == 0u) {
			return polymerPartCounts;
		}
		std::unordered_map<std::pair<char, char>, std::unordered_map<char, long long>, PairHasher> partCountsCache = createCache(steps);
		for (size_t i = 1; i < templatePolymer.size(); i++) {
			addCounts(polymerPartCounts, partCountsCache[{templatePolymer[i - 1], templatePolymer[i]}]);
		}
		return polymerPartCounts;
	}

private:
	void getPolymerPartCounts(std::unordered_map<char, long long>& polymerPartCounts, char in1, char in2, size_t steps) const {
		if (steps == 0u) {
			return;
		}
		char out = getInsertChar(in1, in2);
		polymerPartCounts[out]++;
		getPolymerPartCounts(polymerPartCounts, in1, out, steps - 1);
		getPolymerPartCounts(polymerPartCounts, out, in2, steps - 1);
	}

	std::unordered_map<std::pair<char, char>, std::unordered_map<char, long long>, PairHasher> createCache(size_t steps) const {
		std::unordered_map<std::pair<char, char>, std::unordered_map<char, long long>, PairHasher> partCountsCache;
		for (size_t i = 0; i < steps; i++) {
			std::unordered_map<std::pair<char, char>, std::unordered_map<char, long long>, PairHasher> newPartCountsCache;
			for (const auto& [p, out] : insertionRules) {
				const auto& [in1, in2] = p;
				std::unordered_map<char, long long>& newCache = newPartCountsCache[{in1, in2}];
				addCounts(newCache, partCountsCache[{in1, out}]);
				addCounts(newCache, partCountsCache[{out, in2}]);
				newCache[out]++;
			}
			partCountsCache.swap(newPartCountsCache);
		}
		return partCountsCache;
	}

	std::unordered_map<std::pair<char, char>, char, PairHasher> insertionRules;
};

void printPolymer(const std::list<char>& polymer) {
	for (char c : polymer) {
		std::cout << c;
	}
	std::cout << std::endl;
}

int f1(std::istream& in) {
	std::string templatePolymer;
	in >> templatePolymer;
	in.ignore(10, '\n');

	PolymerInserter inserter;
	std::string line;
	while (std::getline(in, line, '\n')) {
		char in1 = line[0];
		char in2 = line[1];
		char out = line[6];
		inserter.addInsertionRule(in1, in2, out);
	}

	std::list<char> polymer(templatePolymer.begin(), templatePolymer.end());
	for (int i = 0; i < 10; i++) {
		auto before = polymer.begin();
		auto after = polymer.begin();
		after++;
		while (after != polymer.end()) {
			char toInsert = inserter.getInsertChar(*before, *after);
			polymer.insert(after, toInsert);
			before = after;
			after++;
		}
	}
	std::unordered_map<char, int> charCounts;
	for (char c : polymer) {
		charCounts[c]++;
	}
	auto [min, max] = std::minmax_element(charCounts.begin(), charCounts.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.second < rhs.second;
	});

	std::cout << max->second << " - " << min->second << " = " << max->second - min->second << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::string templatePolymer;
	in >> templatePolymer;
	in.ignore(10, '\n');

	PolymerInserter inserter;
	std::string line;
	while (std::getline(in, line, '\n')) {
		char in1 = line[0];
		char in2 = line[1];
		char out = line[6];
		inserter.addInsertionRule(in1, in2, out);
	}
	std::unordered_map<char, long long> charCounts = inserter.getPolymerPartCountsCached(templatePolymer, 40);
	auto [min, max] = std::minmax_element(charCounts.begin(), charCounts.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.second < rhs.second;
	});

	std::cout << max->second << " - " << min->second << " = " << max->second - min->second << std::endl;

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
