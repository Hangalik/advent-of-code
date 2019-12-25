#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <cstring>
#include <map>

constexpr int BUFFER_SIZE = 16;

class OrbitingObject {
public:
	OrbitingObject(bool isCom) : parent{ nullptr }, isCom{ isCom } {
	}

	void addParent(const OrbitingObject * parent) {
		this->parent = parent;
	}

	int orbitCount() const {
		if (isCom) {
			return 0;
		}
		return parent->orbitCount() + 1;
	}

private:
	const OrbitingObject * parent;
	const bool isCom;
};

std::map<std::string, OrbitingObject> orbitingObjects;

void createIfMissing(std::string key) {
	auto it = orbitingObjects.find(key);
	if (it == orbitingObjects.end()) {
		orbitingObjects.emplace(key, OrbitingObject{ strcmp("COM", key.c_str()) == 0 });
	}
}

int main() {

	std::string line;
	while (std::cin >> line) {
		std::stringstream sline{ line };
		char buffer[BUFFER_SIZE];
		sline.getline(buffer, BUFFER_SIZE, ')');
		createIfMissing(buffer);
		const OrbitingObject * parent = &orbitingObjects.at(buffer);
		sline.getline(buffer, BUFFER_SIZE, ')');
		createIfMissing(buffer);
		orbitingObjects.at(buffer).addParent(parent);
	}

	int sum{ 0 };
	for (auto orbitingObject : orbitingObjects) {
		sum += orbitingObject.second.orbitCount();
	}

	std::cout << "Orbit count is " << sum << std::endl;

	return 0;
}
