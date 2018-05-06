#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <FreeImage.h>

class Image {
  public:
    Image(const char *name);
    Image(const std::string& name);

    Image(unsigned width, unsigned height, std::vector<unsigned char> pix);
    Image(unsigned width, unsigned height, unsigned char *pix, unsigned bpp = 32);
    Image(unsigned width, unsigned height, unsigned bpp = 32);

    virtual ~Image();

    void loadData(unsigned width, unsigned height, std::vector<unsigned char> pix);
    void loadData(unsigned width, unsigned height, unsigned char *pix, unsigned bpp = 32);

    void loadFile(const std::string& name);
    void loadFile(const char *name);

    void convertFormat(const size_t newBytesPerPixel);

    unsigned width() const { return _width; }
    unsigned height() const { return _height;  }
    unsigned pitch() const { return _pitch;  }
    unsigned bpp() const { return _bpp;  }
    const std::vector<unsigned char> &data() const { return _pix; }
  protected:
    FIBITMAP *_freeImage = nullptr;
    unsigned _width = 0, _height = 0, _bpp = 32, _pitch = _width;
    std::vector<unsigned char> _pix;
};
