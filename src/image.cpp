#include <FreeImage.h>
#include <stdexcept>
#include <string.h>

#include <iostream>

#include "image.hpp"

Image::Image() : width(0), height(0) {
}

Image::Image(std::string name) {
	this->loadFile(name);
}

Image::Image(unsigned width, unsigned height, unsigned char *temppix) : width(width), height(height), pix(temppix, temppix + width * height * 4) {
}

Image::Image(unsigned width, unsigned height, std::vector<unsigned char> pix) : width(width), height(height), pix(pix) {
}

Image::Image(unsigned width, unsigned height) : Image(width, height, std::vector<unsigned char>(width * height * 4)) {
}

Image::Image(Image const &i) : Image(i.width, i.height, i.pix) {
}

Image::Image(Image &&i) : width(i.width), height(i.height), pix(i.pix) {
	std::swap(width, i.width);
	std::swap(height, i.height);
	std::swap(pix, i.pix);
}

Image &Image::operator=(Image const &i) {
	width = i.width;
	height = i.height;
	pix = i.pix;
	return *this;
}

Image &Image::operator=(Image &&i) {
	std::swap(this->width, i.width);
	std::swap(this->height, i.height);
	std::swap(this->pix, i.pix);
	return *this;
}

void Image::loadFile(std::string filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(format, filename.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	width = FreeImage_GetWidth(imagen);
	height = FreeImage_GetHeight(imagen);
	if(width == 0 || height == 0)
		throw std::runtime_error("Couldn't load image: " + filename);

	unsigned char *temppix = FreeImage_GetBits(imagen);
	this->pix = std::vector<unsigned char>(temppix, temppix + width * height * 4);

	FreeImage_Unload(imagen);
}
