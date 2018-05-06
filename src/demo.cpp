#include "demo.hpp"

#include <GLFW/glfw3.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Demo::init(int argc, char **argv) {
  this->window.makeCurrent();

  this->p = ShaderProgram({FragmentShader("res/test.frag"), VertexShader("res/test.vert")});

  this->p["P"] = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f);
  this->p["V"] = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  this->p["M"] = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));

  verts = Buffer(std::vector<glm::vec3>{
    {+0.0f, -0.5f, 0.0f},
    {-0.5f, +0.5f, 0.0f},
    {+0.5f, +0.5f, 0.0f},
  });
  colors = Buffer(std::vector<glm::vec3>{
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
  });
}

void Demo::update() {
  running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;
}

void Demo::draw() {
  this->window.clearScreen();

  p.use();
  colors.setAttrib(p, "vertexColor");
  verts.setAttrib(p, "vertexPosition");
  verts.drawArrays();
}
