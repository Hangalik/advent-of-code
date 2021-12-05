#include <iostream>
#include <string>

#include "input_selector.h"

int pow(int a, int b) {
	int result = 1;
	for (int i = 0; i < b; i++) {
		result *= a;
	}
	return result;
}

int f1(std::istream& in) {
	std::string line;
	in >> line;
	size_t width = line.size();
	std::vector<int> nums(width);
	int size = 0;
	do {
		size++;
		for (size_t i = 0; i < line.size(); i++) {
			if (line[i] == '1') {
				nums[i]++;
			}
		}
	} while (in >> line);

	int gamma_rate = 0;
	for (int i : nums) {
		gamma_rate *= 2;
		if (i > size - i) {
			gamma_rate++;
		}
	}
	int epsilon_rate = pow(2, width) - gamma_rate - 1;

	std::cout << gamma_rate  << " * " << epsilon_rate << " = " << gamma_rate * epsilon_rate << std::endl;

	return 0;
}

size_t count_of_nth_bit(const std::vector<std::string>& nums, char bit, int n) {
	size_t bit_count = 0;
	for (const std::string& num : nums) {
		if (num[n] == bit) {
			bit_count++;
		}
	}
	return bit_count;
}

char common_nth_bit(const std::vector<std::string>& nums, int n) {
	size_t set_count = count_of_nth_bit(nums, '1', n);
	if (set_count >= nums.size() - set_count) {
		return '1';
	} else {
		return '0';
	}
}

char uncommon_nth_bit(const std::vector<std::string>& nums, int n) {
	size_t set_count = count_of_nth_bit(nums, '1', n);
	if (set_count >= nums.size() - set_count) {
		return '0';
	} else {
		return '1';
	}
}

void print_rates(const std::vector<std::string>& rates) {
	for (const std::string& rate : rates) {
		std::cout << rate << ", ";
	}
	std::cout << std::endl;
}

int f2(std::istream& in) {
	std::vector<std::string> lines;
	for (std::string line; in >> line; ) {
		lines.push_back(std::move(line));
	}

	std::vector<std::string> oxygen_generator_ratings = lines;
	std::vector<std::string> co2_scrubber_ratings = lines;

	int n = 0;
	while (oxygen_generator_ratings.size() > 1) {
		char common_value = common_nth_bit(oxygen_generator_ratings, n);
		std::erase_if(oxygen_generator_ratings, [=](const std::string& rate) {
			return rate[n] != common_value;
		});
		n++;
	}

	n = 0;
	while (co2_scrubber_ratings.size() > 1) {
		char uncommon_value = uncommon_nth_bit(co2_scrubber_ratings, n);
		std::erase_if(co2_scrubber_ratings, [=](const std::string& rate) {
			return rate[n] != uncommon_value;
		});
		n++;
	}

	int oxygen_generator_rating = std::stoi(oxygen_generator_ratings[0], nullptr, 2);
	int co2_scrubber_rating = std::stoi(co2_scrubber_ratings[0], nullptr, 2);
	std::cout << oxygen_generator_rating << " * " << co2_scrubber_rating << " = "
		<< oxygen_generator_rating * co2_scrubber_rating << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	return runWithProperInput(argc, argv, f1, f2);

}