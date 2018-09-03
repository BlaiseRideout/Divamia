#ifndef buffer_H
#define buffer_H

#include "shader.hpp"

#include <glad/glad.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Buffer {
public:
  Buffer();
  Buffer(GLenum target);
  Buffer(Buffer const &b);
  ~Buffer();
  Buffer &operator=(Buffer const &b);
  Buffer &operator=(Buffer &&b);
  operator GLuint();

  template <typename T>
  Buffer(std::vector<T> const &data, GLenum target, GLenum usage) : Buffer() {
    loadData(data, target, usage);
  }

  template <typename T>
  Buffer(std::vector<T> const &data, GLenum target) : Buffer(data, target, GL_STATIC_DRAW) {}

  template <typename T>
  Buffer(std::vector<T> const &data) : Buffer(data, GL_ARRAY_BUFFER) {}

  template <typename T>
  void loadData(std::vector<T> const &data, GLenum target, GLenum usage) {
    if(this->id == 0) {
      glGenBuffers(1, &id);
      Buffer::refCount.insert(std::pair<GLint, unsigned>(id, 1));
    }
    glBindBuffer(target, id);
    glBufferData(target, data.size() * sizeof(T), &data[0], usage);
    this->target = target;
    this->size = data.size();
    setElemSize<T>();
  }

  template <typename T>
  void loadData(std::vector<T> const &data, GLenum target) {
    loadData(data, target, GL_STATIC_DRAW);
  }

  template <typename T>
  void loadData(std::vector<T> const &data) {
    loadData(data, this->target);
  }

  template <typename T>
  void setElemSize() {
    setElemSize(3);
  }

  void setElemSize(int);

  void bind(GLenum target) const;
  void bind() const;
  void unbind(GLenum target) const;
  void unbind() const;

  void setAttrib(GLuint attribute,
                 bool normalized = false,
                 int divisor = 0,
                 GLenum type = GL_FLOAT) const;
  void setAttrib(const ShaderProgram &s,
                 const std::string &name,
                 bool normalized = false,
                 int divisor = 0,
                 GLenum type = GL_FLOAT) const;
  void setAttrib(const ShaderProgram &s,
                 const char *name,
                 bool normalized = false,
                 int divisor = 0,
                 GLenum type = GL_FLOAT) const;

  void drawArrays(GLenum mode = GL_TRIANGLES);
  void drawElements(GLenum mode = GL_TRIANGLES, GLenum type = GL_UNSIGNED_INT);
  void drawInstanced(int count) const;
  void drawInstanced(GLenum mode, int count) const;
  void drawInstanced(GLenum mode, int count, GLenum type) const;

protected:
  void del();

  GLuint id;
  unsigned int size;
  int _elemSize = 3;
  GLenum target;
  static std::map<GLuint, unsigned int> refCount;
};

class IndexBuffer : public Buffer {
public:
  IndexBuffer();
  IndexBuffer(IndexBuffer const &b);
  IndexBuffer(std::vector<unsigned> const &data, GLenum usage);
  IndexBuffer(std::vector<unsigned> const &data);
};

template <>
void Buffer::setElemSize<glm::vec3>();
template <>
void Buffer::setElemSize<glm::vec2>();
template <>
void Buffer::setElemSize<float>();

#endif
