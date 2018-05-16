#include "texture.hpp"

#include <FreeImage.h>
#include <vector>
#include <stdexcept>
#include <iostream>

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, id);
}

Texture::Texture() : id(0), mag_filter(GL_LINEAR), min_filter(GL_LINEAR) {
}

Texture::Texture(Texture const &t) : id(t.id), mag_filter(t.mag_filter), min_filter(t.min_filter) {
  incRef(this->id);
}

Texture::Texture(const char *filename, GLint filter) : Texture(filename, filter, filter) {
}

Texture::Texture(const char *filename, GLint mag_filter, GLint min_filter) : Texture(Image(filename), mag_filter, min_filter) {
}

Texture::Texture(const std::string& filename, GLint filter) : Texture(filename.c_str(), filter) {
}

Texture::Texture(const std::string& filename, GLint mag_filter, GLint min_filter) : Texture(filename.c_str(), mag_filter, min_filter) {
}

Texture::Texture(Image &img, GLint filter) : Texture(img, filter, filter) {
}

Texture::Texture(Image &&img, GLint filter) : Texture(img, filter) {
}

Texture::Texture(Image &img, GLint mag_filter, GLint min_filter) : mag_filter(mag_filter), min_filter(min_filter) {
  img.convertFormat(32);
  glGenTextures(1, &this->id);
  incRef(this->id);

  glBindTexture(GL_TEXTURE_2D, this->id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data().data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::Texture(Image &&img, GLint mag_filter, GLint min_filter) : Texture(img, mag_filter, min_filter) {
}

Texture::Texture(unsigned width, unsigned height, GLint mag_filter, GLint min_filter) : mag_filter(mag_filter), min_filter(min_filter) {
  glGenTextures(1, &this->id);
  incRef(this->id);

  glBindTexture(GL_TEXTURE_2D, this->id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::~Texture() {
  decRef(this->id);
}

Texture &Texture::operator=(Texture const &s) {
  decRef(this->id);
  this->id = s.id;
  min_filter = s.min_filter;
  mag_filter = s.mag_filter;
  incRef(this->id);
  return *this;
}

Texture &Texture::operator=(Image &img) {
  img.convertFormat(32);
  if(!this->id) {
    glGenTextures(1, &this->id);
    incRef(this->id);
  }
  glBindTexture(GL_TEXTURE_2D, this->id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data().data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

  return *this;
}

Texture &Texture::operator=(Image &&img) {
  *this = img;
  return *this;
}

bool Texture::operator==(const Texture &s) {
  return this->id == s.id;
}

Texture::operator GLuint() const {
  return id;
}

unsigned int Texture::width() const {
  glBindTexture(GL_TEXTURE_2D, this->id);
  GLint width;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &width);
  return width;
}

unsigned int Texture::height() const {
  glBindTexture(GL_TEXTURE_2D, this->id);
  GLint height;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  return height;
}

void Texture::decRef(GLuint id) {
  auto iterid = Texture::refCount.find(id);
  if(iterid != Texture::refCount.end()) {
      iterid->second = iterid->second - 1;
      if(iterid->second == 0) {
          glDeleteTextures(1, &id);
          Texture::refCount.erase(iterid);
      }
  }
  else if(id) {
    std::cerr << "Asked to decrement untracked texture!" << std::endl;
  }
}

void Texture::incRef(GLuint id) {
  auto iterid = Texture::refCount.find(id);
  if(iterid != Texture::refCount.end())
      iterid->second = iterid->second + 1;
  else
    Texture::refCount.insert(std::make_pair<GLuint, unsigned int>(std::move(id), 1));
}

std::map<GLuint, unsigned int> Texture::refCount;
