#include <iostream>

#include "input_selector.h"

template <int N>
class Overflow {
public:
	constexpr Overflow(int n) : n(n) {}

	constexpr size_t increment(int m) {
		int intermediate = n + m;
		size_t remain = div_remain(intermediate);
		n = mod(intermediate);
		return remain;
	}

	constexpr int get() const {
		return n;
	}

private:
	static constexpr int mod(int n) {
		n %= N;
		if (n < 0) {
			n += N;
		}
		return n;
	}

	static constexpr size_t div_remain(int n) {
		int result = std::abs(n / N);
		if (n <= 0) {
			result++;
		}
		return result;
	}

	int n;
};

int f1(std::istream& in) {
	Overflow<100> of(50);
	int number_of_zeros = 0;

	std::string line;
	while (std::getline(in, line)) {
		int n = std::stoi(line.substr(1));
		if (line[0] == 'L') {
			n = -n;
		}
		of.increment(n);
		if (of.get() == 0) {
			number_of_zeros++;
		}
	}

	std::cout << number_of_zeros << "\n";

	return 0;
}

int f2(std::istream& in) {
	Overflow<100> of(50);
	size_t number_of_zeros = 0;

	std::string line;
	while (std::getline(in, line)) {
		int n = std::stoi(line.substr(1));
		if (line[0] == 'L') {
			n = -n;
		}
		number_of_zeros += of.increment(n);
		std::cout << line << ": " << of.get() << ", " << number_of_zeros << "\n";
	}

	std::cout << number_of_zeros << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}