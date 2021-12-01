#include <deque>
#include <istream>
#include <iostream>
#include <numeric>
#include <optional>

#include "input_selector.h"

class Summer {
public:
	std::optional<int> newValue(int value) {
		elements.push_back(value);
		if (elements.size() > 3) {
			elements.pop_front();
		}
		if (elements.size() == 3) {
			return sum();
		}
		return std::nullopt;
	}

private:
	int sum() const {
		return std::accumulate(elements.begin(), elements.end(), 0);
	}

	std::deque<int> elements;
};

int f(std::istream& in) {
	Summer summer;
	bool first = true;
	int increaseCount = 0;
	int previous, value;
	while (in >> value) {
		auto sumOpt = summer.newValue(value);
		if (!sumOpt.has_value()) {
			continue;
		}
		int sum = sumOpt.value();
		if (!first && previous < sum) {
			increaseCount++;
		}
		first = false;
		previous = sum;
	}
	
	std::cout << "Increase count: " << increaseCount << std::endl;
	
	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f);
	
}