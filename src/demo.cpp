#include "demo.hpp"

#include <GLFW/glfw3.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void Demo::init(int argc, char **argv) {
  this->window.makeCurrent();

  this->p = ShaderProgram(FragmentShader("res/screen.frag"), VertexShader("res/screen.vert"));

  this->p["P"] = glm::perspective(45.0f, (float)this->window.width / (float)this->window.height, 0.1f, 1000.0f);
  //this->p["V"] = c.viewMatrix();
  //this->p["M"] = glm::translate(glm::mat4(1.0f), boxPosition);

  /*this->t = Texture("res/cubemap.png");
  this->p["tex"] = t;

  this->m = Model("res/cube.obj", this->p, "vertexPosition", "vertexNormal", "vertexUV");*/
}

void Demo::update() {
  running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;
  //c.update();
}

void Demo::draw() {
  this->window.clearScreen();

  //this->p["V"] = c.viewMatrix();

  //this->p["M"] = glm::translate(glm::mat4(1.0f), boxPosition);

  this->p.use();
  //this->m.draw();
}
