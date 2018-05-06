#include "vao.hpp"

#include <iostream>


VAO::VAO() : id(0) {
}

void VAO::bind() {
  if(this->id == 0)
    glGenVertexArrays(1, &this->id);
  glBindVertexArray(this->id);
}

void VAO::unbind() {
  glBindVertexArray(0);
}