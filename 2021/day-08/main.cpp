#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

#include "input_selector.h"

int f1(std::istream& in) {
	constexpr std::streamsize allChars = std::numeric_limits<std::streamsize>::max();
	int countOfUnique = 0;
	while (in.ignore(allChars, '|')) {
		for (int i = 0; i < 4; i++) {
			std::string digitDescriptor;
			in >> digitDescriptor;
			size_t len = digitDescriptor.size();
			if (len == 2 || len == 3 || len == 4 || len == 7) {
				countOfUnique++;
			}
		}
	}

	std::cout << countOfUnique << std::endl;

	return 0;
}

void printSet(const std::set<char>& s) {
	std::cout << "{";
	for (char c : s) {
		std::cout << c << ",";
	}
	std::cout << "}" << std::endl;
}

std::set<char> intersect(const std::set<char>& lhs, const std::set<char>& rhs) {
	std::set<char> resultSet(lhs);
	std::erase_if(resultSet, [&rhs](char c) {
		return !rhs.contains(c);
	});
	/*
	std::cout << "intersect of" << std::endl;
	printSet(lhs);
	std::cout << "and" << std::endl;
	printSet(rhs);
	std::cout << "is" << std::endl;
	printSet(resultSet);*/
	return resultSet;
}

class SegmentPositions {
public:
	SegmentPositions() :
		segmentOptions{
			{'a', allChars},
			{'b', allChars},
			{'c', allChars},
			{'d', allChars},
			{'e', allChars},
			{'f', allChars},
			{'g', allChars}
		}
	{}

	void knownNumber(int num, const std::string& descriptor) {
		segmentsWithDescriptor(segmentCharacters.at(num), descriptor);
	}

	void guessNumber(const std::vector<int>& nums, const std::vector<std::string>& descriptors) {
		if (nums.empty() || descriptors.empty()) {
			return;
		}
		std::set<char> intersectSegmentsOfNums = segmentCharacters.at(nums[0]);
		for (size_t i = 1; i < nums.size(); i++) {
			intersectSegmentsOfNums = intersect(intersectSegmentsOfNums, segmentCharacters.at(nums[i]));
		}
		std::set<char> intersectCharactersOfDescriptor(descriptors[0].begin(), descriptors[0].end());
		for (size_t i = 1; i < descriptors.size(); i++) {
			std::set<char> descriptorSet(descriptors[i].begin(), descriptors[i].end());
			intersectCharactersOfDescriptor = intersect(intersectCharactersOfDescriptor, descriptorSet);
		}
		//printSet(intersectSegmentsOfNums);
		//printSet(intersectCharactersOfDescriptor);
		segmentsWithDescriptorSet(intersectSegmentsOfNums, intersectCharactersOfDescriptor);
	}
	
	int getNumberFromDescriptor(const std::string& descriptor) const {
		std::set<char> realSegments;
		for (const auto& segmentOption : segmentOptions) {
			char fakeSegment = *segmentOption.second.begin();
			if (descriptor.find(fakeSegment) != std::string::npos) {
				realSegments.insert(segmentOption.first);
			}
		}
		for (const auto& segmentCharacter : segmentCharacters) {
			if (realSegments == segmentCharacter.second) {
				return segmentCharacter.first;
			}
		}
		return -1;
	}

	void printSegmentOptions() const {
		std::cout << "segmentOptions{\n";
		for (const auto& value : segmentOptions) {
			std::cout << "\t{" << value.first << ":{";
			for (char c : value.second) {
				std::cout << c << ",";
			}
			std::cout << "}},\n";
		}
		std::cout << "}" << std::endl;
	}

private:
	void segmentsWithDescriptor(const std::set<char>& segmentsInNum, const std::string& descriptor) {
		segmentsWithDescriptorSet(segmentsInNum, std::set<char>(descriptor.begin(), descriptor.end()));
	}
	
	void segmentsWithDescriptorSet(const std::set<char>& segmentsInNum, const std::set<char>& descriptor) {
		if (descriptor.size() != segmentsInNum.size()) {
			throw std::invalid_argument("descriptor is not valid length");
		}
		for (auto& segment : segmentOptions) {
			auto pred = [&](char segmentOption) -> bool {
				bool descriptorContains = descriptor.contains(segmentOption);
				bool segmentBelongsToNum = segmentsInNum.contains(segment.first);
				if (segmentBelongsToNum) {
					return !descriptorContains;
				} else {
					return descriptorContains;
				}
			};
			std::erase_if(segment.second, pred);
		}
	}

	const static std::set<char> allChars;
	const static std::map<int, std::set<char>> segmentCharacters;

	std::map<char, std::set<char>> segmentOptions;
};

const std::set<char> SegmentPositions::allChars{ 'a', 'b', 'c', 'd', 'e', 'f', 'g' };

/*
*   0:      1:      2:      3:      4:
*  aaaa    ....    aaaa    aaaa    ....
* b    c  .    c  .    c  .    c  b    c
* b    c  .    c  .    c  .    c  b    c
*  ....    ....    dddd    dddd    dddd
* e    f  .    f  e    .  .    f  .    f
* e    f  .    f  e    .  .    f  .    f
*  gggg    ....    gggg    gggg    ....
*
*   5:      6:      7:      8:      9:
*  aaaa    aaaa    aaaa    aaaa    aaaa
* b    .  b    .  .    c  b    c  b    c
* b    .  b    .  .    c  b    c  b    c
*  dddd    dddd    ....    dddd    dddd
* .    f  e    f  .    f  e    f  .    f
* .    f  e    f  .    f  e    f  .    f
*  gggg    gggg    ....    gggg    gggg
*/

const std::map<int, std::set<char>> SegmentPositions::segmentCharacters{
	{0, {'a', 'b', 'c', 'e', 'f', 'g'}},
	{1, {'c', 'f'}},
	{2, {'a', 'c', 'd', 'e', 'g'}},
	{3, {'a', 'c', 'd', 'f', 'g'}},
	{4, {'b', 'c', 'd', 'f'}},
	{5, {'a', 'b', 'd', 'f', 'g'}},
	{6, {'a', 'b', 'd', 'e', 'f', 'g'}},
	{7, {'a', 'c', 'f'}},
	{8, {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
	{9, {'a', 'b', 'c', 'd', 'f', 'g'}}
};

int f2(std::istream& in) {
	std::string line;
	int sumOfResults = 0;
	while (std::getline(in, line)) {
		std::istringstream strstream(std::move(line));
		std::vector<std::string> clues(10);
		for (int i = 0; i < 10; i++) {
			strstream >> clues[i];
		}
		strstream.ignore(2);

		SegmentPositions sp;
		std::vector<std::string> fivePieceClues;
		std::vector<std::string> sixPieceClues;
		for (const std::string& clue : clues) {
			switch (clue.length()) {
			case 2u:
				sp.knownNumber(1, clue);
				break;
			case 3u:
				sp.knownNumber(7, clue);
				break;
			case 4u:
				sp.knownNumber(4, clue);
				break;
			case 5u:
				fivePieceClues.push_back(clue);
				break;
			case 6u:
				sixPieceClues.push_back(clue);
				break;
			default:
				break;
			}
		}
		sp.guessNumber({2, 3, 5}, fivePieceClues);
		sp.guessNumber({0, 6, 9}, sixPieceClues);
		
		int result = 0;
		for (int i = 0; i < 4; i++) {
			std::string descriptor;
			strstream >> descriptor;
			result *= 10;
			result += sp.getNumberFromDescriptor(descriptor);
		}
		sumOfResults += result;
	}
	
	std::cout << sumOfResults << std::endl;

	return 0;
}

int main(int argc, const char* argv[]) {

	try {
		return runWithProperInput(argc, argv, f1, f2);
	} catch (std::invalid_argument& le) {
		std::cerr << le.what() << std::endl;
		return 1;
	}
	
}