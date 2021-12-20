#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <span>

#include "input_selector.h"

using ImageData = std::vector<std::vector<bool>>;

static constexpr char lightPixel = '#';
static constexpr char darkPixel = '.';

class Image {
public:
	explicit Image(ImageData imageData) : imageData(std::move(imageData)), infinite(false) {}

	void enhance(std::span<bool, 512> lookupTable) {
		size_t rowCount = imageData.size();
		size_t rowLength = imageData[0].size();
		ImageData newData(rowCount + 2, std::vector<bool>(rowLength + 2));
		for (size_t i = 0; i < newData.size(); i++) {
			for (size_t j = 0; j < newData[i].size(); j++) {
				int x = j - 1;
				int y = i - 1;
				std::array<bool, 9> window;
				window[0] = getDataAt(x - 1, y - 1);
				window[1] = getDataAt(x,     y - 1);
				window[2] = getDataAt(x + 1, y - 1);
				window[3] = getDataAt(x - 1, y);
				window[4] = getDataAt(x,     y);
				window[5] = getDataAt(x + 1, y);
				window[6] = getDataAt(x - 1, y + 1);
				window[7] = getDataAt(x,     y + 1);
				window[8] = getDataAt(x + 1, y + 1);
				size_t lookupIdx = 0;
				for (bool b : window) {
					lookupIdx *= 2;
					lookupIdx += b;
				}
				newData[i][j] = lookupTable[lookupIdx];
			}
		}
		infinite = getNewInfinite(lookupTable);
		imageData = std::move(newData);
	}

	int getLightPixelCount() const {
		int lightCount = 0;
		for (const auto& imgRow : imageData) {
			for (bool pixel : imgRow) {
				if (pixel) {
					lightCount++;
				}
			}
		}
		return lightCount;
	}

	void printImage() const {
		char infiniteChar = getInfinitePixel();
		std::string infiniteLine(imageData[0].size() + 2, infiniteChar);
		std::cout << infiniteLine << std::endl;
		for (const auto& imgRow : imageData) {
			std::cout << infiniteChar;
			for (bool pixel : imgRow) {
				if (pixel) {
					std::cout << lightPixel;
				} else {
					std::cout << darkPixel;
				}
			}
			std::cout << infiniteChar << std::endl;
		}
		std::cout << infiniteLine << std::endl;
	}

private:
	bool getDataAt(int x, int y) const {
		int rowCount = imageData.size();
		int rowLength = imageData[0].size();
		if (x < 0 || y < 0 ||
				x >= rowLength || y >= rowCount) {
			return infinite;
		}
		return imageData[y][x];
	}

	char getInfinitePixel() const {
		if (infinite) {
			return lightPixel;
		} else {
			return darkPixel;
		}
	}

	bool getNewInfinite(const std::span<bool, 512>& lookupTable) const {
		if (infinite) {
			return lookupTable[511];
		} else {
			return lookupTable[0];
		}
	}

	ImageData imageData;
	bool infinite;
};

int f1(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');
	std::array<bool, 512> lookupTable;
	for (size_t i = 0; i < line.size(); i++) {
		lookupTable[i] = line[i] == lightPixel;
	}
	ImageData imgData;
	while (std::getline(in, line, '\n')) {
		if (line.empty()) continue;

		std::vector<bool> imgRow(line.size());
		for (size_t i = 0; i < line.size(); i++) {
			imgRow[i] = line[i] == lightPixel;
		}
		imgData.push_back(std::move(imgRow));
	}
	Image img(std::move(imgData));
	img.enhance(lookupTable);
	img.enhance(lookupTable);

	std::cout << img.getLightPixelCount() << std::endl;

	return 0;
}

int f2(std::istream& in) {
	std::string line;
	std::getline(in, line, '\n');
	std::array<bool, 512> lookupTable;
	for (size_t i = 0; i < line.size(); i++) {
		lookupTable[i] = line[i] == lightPixel;
	}
	ImageData imgData;
	while (std::getline(in, line, '\n')) {
		if (line.empty()) continue;

		std::vector<bool> imgRow(line.size());
		for (size_t i = 0; i < line.size(); i++) {
			imgRow[i] = line[i] == lightPixel;
		}
		imgData.push_back(std::move(imgRow));
	}
	Image img(std::move(imgData));
	for (int i = 0; i < 50; i++) {
		img.enhance(lookupTable);
	}
	std::cout << img.getLightPixelCount() << std::endl;

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
