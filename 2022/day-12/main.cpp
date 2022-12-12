#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>
#include <limits>

#include "input_selector.h"

int getHeightt(char c) {
	if (c == 'S') {
		c = 'a';
	}
	if (c == 'E') {
		c = 'z';
	}
	return c - 'a';
}

class Square {
public:
	explicit Square(char c) : height(getHeightt(c)) {}

	int getHeight() const {
		return height;
	}

	std::vector<std::weak_ptr<Square>> getNeighbours() const {
		return neighbours;
	}

	void addNeighbour(std::weak_ptr<Square> neighbour) {
		neighbours.push_back(std::move(neighbour));
	}

private:
	int height;
	std::vector<std::weak_ptr<Square>> neighbours;
};

void addNeighbours(std::shared_ptr<Square>& sq1, std::shared_ptr<Square>& sq2) {
	if (sq1->getHeight() + 1 >= sq2->getHeight()) {
		sq1->addNeighbour(sq2);
	}
	if (sq2->getHeight() + 1 >= sq1->getHeight()) {
		sq2->addNeighbour(sq1);
	}
}

std::tuple<std::vector<std::shared_ptr<Square>>, std::shared_ptr<Square>, std::shared_ptr<Square>>
	readInSquares(std::istream& in)
{
	std::vector<std::shared_ptr<Square>> squares;
	std::string line;
	std::shared_ptr<Square> start, end;
	while (std::getline(in, line)) {
		for (size_t i = 0; i < line.size(); i++) {
			char c = line[i];
			auto square_ptr = std::make_shared<Square>(c);
			if (c == 'E') {
				end = square_ptr;
			} else if (c == 'S') {
				start = square_ptr;
			}
			if (i > 0) {
				addNeighbours(square_ptr, squares.back());
			}
			int upNeighbourIdx = squares.size() - line.size();
			if (upNeighbourIdx >= 0) {
				addNeighbours(square_ptr, squares.at(upNeighbourIdx));
			}
			squares.push_back(std::move(square_ptr));
		}
	}
	return {squares, start, end};
}

int shortestPath(const std::shared_ptr<Square>& start, const std::shared_ptr<Square>& end) {
	std::unordered_set<std::shared_ptr<Square>> visitedSquares;
	std::unordered_set<std::shared_ptr<Square>> perimeter{start};
	int depth = 0;
	bool endFound = false;
	while (!endFound) {
		if (perimeter.empty()) {
			return std::numeric_limits<int>::max();
		}
		std::unordered_set<std::shared_ptr<Square>> newPerimeter;
		for (auto sq : perimeter) {
			std::vector<std::weak_ptr<Square>> neighbours = sq->getNeighbours();
			for (auto neighbour : neighbours) {
				if (auto neighbour_shared = neighbour.lock()) {
					if (neighbour_shared == end) {
						endFound = true;
					}
					if (!perimeter.contains(neighbour_shared) && !visitedSquares.contains(neighbour_shared)) {
						newPerimeter.insert(neighbour_shared);
					}
				}
			}
		}
		visitedSquares.insert(perimeter.begin(), perimeter.end());
		perimeter = std::move(newPerimeter);
		depth++;
	}
	return depth;
}

int f1(std::istream& in) {
	auto [squares, start, end] = readInSquares(in);
	int depth = shortestPath(start, end);

	std::cout << depth << "\n";

	return 0;
}

int f2(std::istream& in) {
	auto [squares, start, end] = readInSquares(in);

	auto extraSquare = std::make_shared<Square>('a');
	for (auto sq : squares) {
		if (sq->getHeight() == 0) {
			extraSquare->addNeighbour(sq);
		}
	}
	int depth = shortestPath(extraSquare, end);
	depth--;

	std::cout << depth << "\n";

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