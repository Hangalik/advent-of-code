#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <optional>
#include <compare>
#include <iterator>
#include <ranges>
#include <algorithm>

#include "input_selector.h"

class Block {
public:
	Block() : block(std::nullopt) {}
	Block(int id) : block(id) {}

	bool isFree() const { return !block.has_value(); }
	int getId() const { return block.value(); }

private:
	std::optional<int> block;
};

class DiskMap {
	class Entry {
	public:
		explicit Entry(int length) : length(length), block() {}
		explicit Entry(int length, int id) : length(length), block(id) {}

		int getLength() const { return length; }
		const Block& getBlock() const { return block; }
		Block& getBlock() { return block; }

	private:
		int length;
		Block block;
	};

	struct File { int blockStart; int blockSize; int fileId; };

public:
	class iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Block;
		using pointer           = Block*;
		using reference         = Block&;

		explicit iterator(std::vector<Entry>::iterator it) : currentEntry(it), currentIndex(0) {}

		iterator& operator++() {
			if (currentIndex + 1 < currentEntry->getLength()) {
				currentIndex++;
			} else {
				do {
					currentEntry++;
				} while (currentEntry->getLength() <= 0);
				currentIndex = 0;
			}
			return *this;
		}

		iterator operator++(int) {
			iterator me = *this;
			this->operator++();
			return me;
		}

		iterator& operator--() {
			if (currentIndex > 0) {
				currentIndex--;
			} else {
				do {
					currentEntry--;
				} while (currentEntry->getLength() <= 0);
				currentIndex = currentEntry->getLength() - 1;
			}
			return *this;
		}

		iterator operator--(int) {
			iterator me = *this;
			this->operator--();
			return me;
		}

		const Block& operator*() const {
			return currentEntry->getBlock();
		}

		Block& operator*() {
			return currentEntry->getBlock();
		}

		const Block* operator->() const {
			return &currentEntry->getBlock();
		}

		Block* operator->() {
			return &currentEntry->getBlock();
		}

		// friend std::strong_ordering operator<=>(const iterator& lhs, const iterator& rhs) {
		// 	auto comp = (lhs.currentEntry <=> rhs.currentEntry);
		// 	if (comp != 0) {
		// 		return comp;
		// 	} else {
		// 		return lhs.currentIndex <=> rhs.currentIndex;
		// 	}
		// }

		friend bool operator==(const iterator& lhs, const iterator& rhs) {
			return lhs.currentEntry == rhs.currentEntry && lhs.currentIndex == rhs.currentIndex;
		}

	private:
		std::vector<Entry>::iterator currentEntry;
		int currentIndex;
	};

	explicit DiskMap(const std::string& map) {
		int id = 0;
		bool freeSpace = false;
		for (char c : map) {
			int length = c - '0';
			if (freeSpace) {
				entries.emplace_back(length);
			} else {
				entries.emplace_back(length, id);
				id++;
			}
			freeSpace = !freeSpace;
		}
	}

	void compact() {
		std::vector<File> files = getFileFormat();
		auto occupyRange = std::views::transform(files, [](const File& file) { return std::make_pair(file.blockStart, file.blockSize); });
		std::set<std::pair<int, int>> occupiedSpaces(occupyRange.begin(), occupyRange.end());

		for (File& file : std::views::reverse(files)) {
			int requiredSpace = file.blockSize;
			for (const auto& [former, later] : std::views::pairwise(occupiedSpaces)) {
				if (later.first >= file.blockStart) {
					break;
				}
				int freeSpaceBetween = later.first - (former.first + former.second);
				if (freeSpaceBetween >= requiredSpace) {
					file.blockStart = former.first + former.second;
					occupiedSpaces.emplace(file.blockStart, file.blockSize);
					break;
				}
			}
		}
		std::ranges::sort(files, {}, &File::blockStart);
		entries.clear();
		for (const auto& [f1, f2] : std::views::pairwise(files)) {
			entries.emplace_back(f1.blockSize, f1.fileId);
			int freeSpaceBetween = f2.blockStart - (f1.blockStart + f1.blockSize);
			if (freeSpaceBetween > 0) {
				entries.emplace_back(freeSpaceBetween);
			}
		}
		entries.emplace_back(files.back().blockSize, files.back().fileId);
	}

	iterator begin() { return iterator(entries.begin()); }
	iterator end() { return iterator(entries.end()); }
	std::reverse_iterator<iterator> rbegin() { return std::reverse_iterator(end()); }
	std::reverse_iterator<iterator> rend() { return std::reverse_iterator(begin()); }

private:
	std::vector<File> getFileFormat() const {
		std::vector<File> files;
		int block = 0;
		for (const Entry& entry : entries) {
			if (!entry.getBlock().isFree()) {
				files.emplace_back(block, entry.getLength(), entry.getBlock().getId());
			}
			block += entry.getLength();
		}
		return files;
	}

	std::vector<Entry> entries;
};

long long computeChecksum(DiskMap& disk) {
	auto front = disk.begin();
	auto back = disk.rbegin();

	int idx = 0;
	long long sum = 0;
	while (front != back.base()) {
		if (front->isFree()) {
			if (back->isFree()) {
				back++;
				continue;
			} else {
				sum += idx * back->getId();
				std::cout << "+ " << idx << " * " << back->getId() << " = " << sum << "\n";
				back++;
			}
		} else {
			sum += idx * front->getId();
			std::cout << "+ " << idx << " * " << front->getId() << " = " << sum << "\n";
		}
		idx++;
		front++;
	}
	// std::cout << "\n";
	return sum;
}

int f1(std::istream& in) {
	std::string line;
	std::getline(in, line);

	DiskMap disk(line);
	long long sum = computeChecksum(disk);
	std::cout << sum << "\n";

	return 0;
}

int f2(std::istream& in) {
	std::string line;
	std::getline(in, line);

	DiskMap disk(line);
	disk.compact();

	for (Block &b : disk) {
		if (b.isFree()) {
			std::cout << '.';
		} else {
			std::cout << b.getId();
		}
	}
	std::cout << "\n";

	long long sum = computeChecksum(disk);
	std::cout << sum << "\n";

	return 0;
}

int main(int argc, const char* argv[]) {
	return runWithProperInput(argc, argv, f1, f2);
}