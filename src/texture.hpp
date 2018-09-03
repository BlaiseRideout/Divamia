#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.hpp"

#include <glad/glad.h>
#include <map>
#include <string>

class Texture {
public:
  Texture();
  Texture(unsigned width, unsigned height, GLint mag_filter, GLint min_filter);
  Texture(Texture const &t);
  Texture(Texture &&t) = default;

  Texture(const char *name, GLint filter = GL_NEAREST);
  Texture(const char *name, GLint mag_filter, GLint min_filter);

  Texture(const std::string &name, GLint filter = GL_NEAREST);
  Texture(const std::string &name, GLint mag_filter, GLint min_filter);

  Texture(Image &img, GLint filter = GL_NEAREST);
  Texture(Image &img, GLint mag_filter, GLint min_filter);
  Texture(Image &&img, GLint filter = GL_NEAREST);
  Texture(Image &&img, GLint mag_filter, GLint min_filter);

  virtual ~Texture();
  Texture &operator=(Texture const &s);
  Texture &operator=(Texture &&s) = default;
  Texture &operator=(Image &i);
  Texture &operator=(Image &&i);
  bool operator==(const Texture &s);
  operator GLuint() const;

  unsigned int width() const;
  unsigned int height() const;
  void bind() const;

protected:
  GLuint id;
  GLint mag_filter;
  GLint min_filter;

  static void decRef(GLuint id);
  static void incRef(GLuint id);

  static std::map<GLuint, unsigned int> refCount;
};

#endif
