#include <iostream>
#include <memory>
#include <vector>
#include <limits>

#include "input_selector.h"

class Directory;

class File {
public:
	File(std::weak_ptr<Directory> parent, int size) :
			parent(std::move(parent)),
			size(size)
	{}

	int getSize() const {
		return size;
	}

	std::weak_ptr<Directory> getParent() const {
		return parent;
	}

private:
	std::weak_ptr<Directory> parent;
	int size;
};

struct DirectoryVisitor {
	virtual void visit(const Directory& dir) = 0;
};

class Directory {
public:
	Directory(std::weak_ptr<Directory> parent, std::string name) :
			parent(std::move(parent)),
			name(std::move(name))
	{}

	int getSize() const {
		int size = 0;
		for (const auto& childFile : childrenFiles) {
			size += childFile->getSize();
		}
		for (const auto& childDir : childrenDirs) {
			size += childDir->getSize();
		}
		return size;
	}

	std::weak_ptr<Directory> getParent() const {
		return parent;
	}

	const std::string& getName() const {
		return name;
	}

	std::shared_ptr<Directory> getDirWithName(const std::string& name) const {
		for (const auto& childDir : childrenDirs) {
			if (childDir->getName() == name) {
				return childDir;
			}
		}
		throw std::logic_error("no child wiht name");
	}

	void addChildFile(File&& file) {
		auto sharedFile = std::make_shared<File>(std::move(file));
		childrenFiles.push_back(std::move(sharedFile));
	}

	void addChildDir(Directory&& dir) {
		auto sharedDirectory = std::make_shared<Directory>(std::move(dir));
		childrenDirs.push_back(std::move(sharedDirectory));
	}

	void visitDirs(DirectoryVisitor& visitor) const {
		visitor.visit(*this);
		for (const auto& childDir : childrenDirs) {
			childDir->visitDirs(visitor);
		}
	}

private:
	std::vector<std::shared_ptr<File>> childrenFiles;
	std::vector<std::shared_ptr<Directory>> childrenDirs;
	std::weak_ptr<Directory> parent;
	std::string name;
};

std::shared_ptr<Directory> readDirTree(std::istream& in) {
	std::shared_ptr<Directory> root = std::make_shared<Directory>(std::weak_ptr<Directory>{}, "/");
	std::shared_ptr<Directory> currentWd = root;
	std::string line;
	while (std::getline(in, line)) {
		if (line[0] == '$') {
			if (line[2] == 'l') {
				continue;
			}
			std::string dirName = line.substr(5);
			if (dirName == "/") {
				currentWd = root;
			} else if (dirName == "..") {
				std::shared_ptr<Directory> parent(currentWd->getParent());
				if (!bool(parent)) {
					throw std::logic_error("no parent");
				}
				currentWd = parent;
			} else {
				currentWd = currentWd->getDirWithName(dirName);
			}
		} else {
			if (std::isdigit(line[0])) {
				int size = std::stoi(line.substr(0, line.find(' ')));
				currentWd->addChildFile(File{currentWd, size});
			} else {
				std::string name = line.substr(4);
				currentWd->addChildDir(Directory{currentWd, name});
			}
		}
	}
	return root;
}

struct LimitedSizeCumulator : public DirectoryVisitor {
	void visit(const Directory& dir) override {
		int dirSize = dir.getSize();
		if (dirSize <= 100000) {
			size += dirSize;
		}
	}

	int size = 0;
};

int f1(std::istream& in) {
	std::shared_ptr<Directory> root = readDirTree(in);

	LimitedSizeCumulator lsc;
	root->visitDirs(lsc);
	std::cout << lsc.size << "\n";

	return 0;
}

struct MinSizeOfAtLeastFinder : public DirectoryVisitor {
	MinSizeOfAtLeastFinder(int leastSize) : leastSize(leastSize) {}

	void visit(const Directory& dir) override {
		int dirSize = dir.getSize();
		if (dirSize >= leastSize) {
			minSize = std::min(minSize, dirSize);
		}
	}

	int leastSize;
	int minSize = std::numeric_limits<int>::max();
};

int f2(std::istream& in) {
	std::shared_ptr<Directory> root = readDirTree(in);

	const int max_size = 70000000;
	const int free_needed = 30000000;

	int current_free = max_size - root->getSize();
	int delete_at_least = free_needed - current_free;

	MinSizeOfAtLeastFinder finder(delete_at_least);
	root->visitDirs(finder);
	std::cout << finder.minSize << "\n";

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