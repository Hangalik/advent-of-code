#include <iostream>
#include <limits>
#include <array>

constexpr int width = 25;
constexpr int height = 6;

void printImage(const std::array<int, width * height> image) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (image[i * width + j] == 1) {
				std::cout << "0";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

int main() {

	std::array<int, width * height> image;
	image.fill(2);

	while (true) {
		for (int i = 0; i < width * height; i++) {
			char c;
			if (!std::cin.get(c)) {
				printImage(image);
				return 0;
			}
			switch (c) {
			case '0':
				if (image[i] == 2) {
					image[i] = 0;
				}
				break;
			case '1':
				if (image[i] == 2) {
					image[i] = 1;
				}
				break;
			case '2':
				break;
			default:
				break;
			}
		}
	}

	return 0;
}
