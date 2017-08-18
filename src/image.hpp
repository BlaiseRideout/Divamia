#pragma once

#include <vector>
#include <string>

class Image {
	public:
		Image();
		Image(std::string name);
		Image(unsigned width, unsigned height, std::vector<unsigned char> pix);
		Image(unsigned width, unsigned height, unsigned char *pix);
		Image(unsigned width, unsigned height);
		Image(Image const &i);
		Image(Image &&i);
		void loadFile(std::string name);

		Image &operator=(Image const &i);
		Image &operator=(Image &&i);

		unsigned width, height;
		std::vector<unsigned char> pix;
};
