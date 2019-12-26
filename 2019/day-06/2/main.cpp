#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <cstring>
#include <map>

constexpr int BUFFER_SIZE = 16;

class OrbitingObject {
public:
	OrbitingObject() : parent{ nullptr } {
	}

	void addParent(const OrbitingObject * parent) {
		this->parent = parent;
	}

	int orbitCount() const {
		if (parent == nullptr) {
			return 0;
		}
		return parent->orbitCount() + 1;
	}

	const OrbitingObject * findCommonAncestor(const OrbitingObject * other) {
		if (this == other) {
			return this;
		}
		if (other == nullptr) {
			throw "No common ancestor";
		}
		auto currentlyLookedAt{ parent };
		while (currentlyLookedAt != nullptr) {
			if (currentlyLookedAt == other) {
				return currentlyLookedAt;
			}
			currentlyLookedAt = currentlyLookedAt->parent;
		}
		return findCommonAncestor(other->parent);
		
	}

private:
	const OrbitingObject * parent;
};

int main() {

	std::map<std::string, OrbitingObject> orbitingObjects;
	std::string line;
	while (std::cin >> line) {
		std::stringstream sline{ line };
		char buffer[BUFFER_SIZE];
		sline.getline(buffer, BUFFER_SIZE, ')');
		const OrbitingObject * parent = &orbitingObjects[buffer];
		sline.getline(buffer, BUFFER_SIZE, ')');
		orbitingObjects[buffer].addParent(parent);
	}

	auto closestCommonAncestor{ orbitingObjects["YOU"].findCommonAncestor(&orbitingObjects["SAN"]) };

	int yourOrbitCount{ orbitingObjects["YOU"].orbitCount() };
	int santasOrbitCount{ orbitingObjects["SAN"].orbitCount() };
	int commonAncestorOrbitCount{ closestCommonAncestor->orbitCount() };

	std::cout << "Your orbit count is " << yourOrbitCount << std::endl;
	std::cout << "Santas orbit count is " << santasOrbitCount << std::endl;
	std::cout << "Your and Santas common orbit object's orbit count is " << commonAncestorOrbitCount << std::endl;
	std::cout << "Transfers needed: " << ((yourOrbitCount - 1) - commonAncestorOrbitCount) + ((santasOrbitCount - 1) - commonAncestorOrbitCount) << std::endl;

	return 0;
}
