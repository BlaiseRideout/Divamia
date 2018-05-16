#include "image.hpp"

#include <cassert>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <array>

static constexpr size_t BITS_PER_BYTE = 8;

static std::unordered_map<FREE_IMAGE_FORMAT, const char*> formatStrings{
  { FIF_UNKNOWN, "FIF_UNKNOWN" },
  { FIF_BMP, "FIF_BMP" },
  { FIF_ICO, "FIF_ICO" },
  { FIF_JPEG, "FIF_JPEG" },
  { FIF_JNG, "FIF_JNG" },
  { FIF_KOALA, "FIF_KOALA" },
  { FIF_LBM, "FIF_LBM" },
  { FIF_IFF, "FIF_IFF" },
  { FIF_MNG, "FIF_MNG" },
  { FIF_PBM, "FIF_PBM" },
  { FIF_PBMRAW, "FIF_PBMRAW" },
  { FIF_PCD, "FIF_PCD" },
  { FIF_PCX, "FIF_PCX" },
  { FIF_PGM, "FIF_PGM" },
  { FIF_PGMRAW, "FIF_PGMRAW" },
  { FIF_PNG, "FIF_PNG" },
  { FIF_PPM, "FIF_PPM" },
  { FIF_PPMRAW, "FIF_PPMRAW" },
  { FIF_RAS, "FIF_RAS" },
  { FIF_TARGA, "FIF_TARGA" },
  { FIF_TIFF, "FIF_TIFF" },
  { FIF_WBMP, "FIF_WBMP" },
  { FIF_PSD, "FIF_PSD" },
  { FIF_CUT, "FIF_CUT" },
  { FIF_XBM, "FIF_XBM" },
  { FIF_XPM, "FIF_XPM" },
  { FIF_DDS, "FIF_DDS" },
  { FIF_GIF, "FIF_GIF" },
  { FIF_HDR, "FIF_HDR" },
  { FIF_FAXG3, "FIF_FAXG3" },
  { FIF_SGI, "FIF_SGI" },
  { FIF_EXR, "FIF_EXR" },
  { FIF_J2K, "FIF_J2K" },
  { FIF_JP2, "FIF_JP2" },
  { FIF_PFM, "FIF_PFM" },
  { FIF_PICT, "FIF_PICT" },
  { FIF_RAW, "FIF_RAW" },
};

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
    _width,
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
  if(format == FIF_UNKNOWN)
    format = FreeImage_GetFIFFromFilename(filename);

  _freeImage = FreeImage_Load(format, filename);

  if (format == FIF_UNKNOWN) {
    std::string message("Can't identify image format for: ");
    message += filename;
    throw std::runtime_error(message);
  }

  _bpp = FreeImage_GetBPP(_freeImage);
  _width = FreeImage_GetWidth(_freeImage);
  _height = FreeImage_GetHeight(_freeImage);

  if (_width == 0 || _height == 0) {
    std::string message("Couldn't load image: ");
    message += filename;
    throw std::runtime_error(message);
  }

  FreeImage_FlipVertical(_freeImage);

  unsigned char *temppix = FreeImage_GetBits(_freeImage);
  _pix = std::vector<unsigned char>(temppix, temppix + _width * _height * _bpp / BITS_PER_BYTE);
}

void Image::convertFormat(const size_t newBPP) {
  if(newBPP == _bpp)
    return;

  FIBITMAP* oldImage = _freeImage;
  switch(newBPP) {
  case 32:
    _freeImage = FreeImage_ConvertTo32Bits(oldImage);
    break;
  case 24:
    _freeImage = FreeImage_ConvertTo24Bits(oldImage);
    break;
  default:
    throw std::runtime_error("Couldn't convert image. Unsupported BPP requested");
  }
  _bpp = newBPP;
  FreeImage_Unload(oldImage);
}
