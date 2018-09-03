#ifndef VAO_H
#define VAO_H

#include "buffer.hpp"
#include "shader.hpp"

class VAO {
public:
  VAO();

  void bind();
  void unbind();
  operator GLuint();

protected:
  GLuint id;
};

#endif
