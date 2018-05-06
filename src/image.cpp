#include "image.hpp"

#include <cassert>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <array>

static constexpr size_t BITS_PER_BYTE = 8;

static const std::unordered_map<unsigned, std::array<unsigned, 3>> masks{
  {32, {0xFF0000, 0x00FF00, 0x0000FF} },
  {24, {0xFF0000, 0x00FF00, 0x0000FF} },
};

Image::Image(const char *name) {
  loadFile(name);
}

Image::Image(const std::string& name) : Image(name.c_str()) {
}

Image::Image(unsigned width, unsigned height, std::vector<unsigned char> pix) {
  loadData(width, height, std::move(pix));
}

Image::Image(unsigned width, unsigned height, unsigned char *temppix, unsigned bpp)  : Image(width, height, std::vector<unsigned char>(temppix, temppix + width * height * bpp / BITS_PER_BYTE)) {
}

Image::Image(unsigned width, unsigned height, unsigned bpp) : Image(width, height, std::vector<unsigned char>(width * height * bpp / BITS_PER_BYTE)) {
}

Image::~Image() {
  if(_freeImage != nullptr)
    FreeImage_Unload(_freeImage);
  _freeImage = nullptr;
}

void Image::loadData(unsigned width, unsigned height, std::vector<unsigned char> pix) {
  _width = width;
  _pitch = width;
  _height = height;
  _pix = std::move(pix);
  _bpp = _pix.size() / _width / _height * BITS_PER_BYTE;

  assert(masks.find(_bpp) != masks.end());

  if(_freeImage != nullptr)
    FreeImage_Unload(_freeImage);

  _freeImage = FreeImage_ConvertFromRawBits(
    _pix.data(),
    _width,
    _height,
    _pitch,
    _bpp,
    masks.at(_bpp)[0], // Red mask
    masks.at(_bpp)[1], // Green mask
    masks.at(_bpp)[2]  // Blue mask
  );
}

void Image::loadData(unsigned width, unsigned height, unsigned char *pix, unsigned bpp) {
  loadData(width, height, std::vector<unsigned char>(width * height * bpp / BITS_PER_BYTE));
}

void Image::loadFile(const std::string& filename) {
  loadFile(filename.c_str());
}

void Image::loadFile(const char *filename) {
  if(_freeImage != nullptr)
    FreeImage_Unload(_freeImage);

  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
  _freeImage = FreeImage_Load(format, filename);

  _bpp = FreeImage_GetBPP(_freeImage);
  _pitch = FreeImage_GetPitch(_freeImage);
  _width = FreeImage_GetWidth(_freeImage);
  _height = FreeImage_GetHeight(_freeImage);

  if (_width == 0 || _height == 0) {
    std::string message("Couldn't load image: ");
    message += filename;
    throw std::runtime_error(message);
  }

  unsigned char *temppix = FreeImage_GetBits(_freeImage);
  _pix = std::vector<unsigned char>(temppix, temppix + _pitch * _height * _bpp / BITS_PER_BYTE);
}

void Image::convertFormat(const size_t newBytesPerPixel) {
  assert(false);
}
