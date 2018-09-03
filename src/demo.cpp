#include "demo.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

void Demo::init(int argc, char **argv) {
  window.makeCurrent();

  initTriangle();
  initTexture();
}

void Demo::initTexture() {
  texture = ShaderProgram({FragmentShader("res/texture.frag"), VertexShader("res/texture.vert")});

  Texture tex("./res/fish_sandwich_small.png");
  texture["tex"] = tex;
  texture["P"] =
      glm::ortho(0.0f, static_cast<float>(window.width), static_cast<float>(window.height), 0.0f);
  texture["V"] = glm::mat4(1.0f);

  static constexpr float displayWidth = 200.0f;

  texture["M"] = glm::translate(
      glm::scale(glm::mat4(1.0f),
                 glm::vec3(displayWidth, displayWidth * tex.height() / tex.width(), 0.0f)),
      glm::vec3(1.5f, 1.5f, 0.0f));

  textureVerts = Buffer(std::vector<glm::vec3>{
      {+0.5f, -0.5f, 0.0f}, // UR
      {-0.5f, +0.5f, 0.0f}, // LL
      {+0.5f, +0.5f, 0.0f}, // LR

      {+0.5f, -0.5f, 0.0f}, // UR
      {-0.5f, -0.5f, 0.0f}, // UL
      {-0.5f, +0.5f, 0.0f}, // LL
  });

  textureUVs = Buffer(std::vector<glm::vec2>{
      {1.0f, 0.0f}, // UR
      {0.0f, 1.0f}, // LL
      {1.0f, 1.0f}, // LR

      {1.0f, 0.0f}, // UR
      {0.0f, 0.0f}, // UL
      {0.0f, 1.0f}, // LL
  });
}

void Demo::initTriangle() {
  triangle = ShaderProgram({FragmentShader("res/test.frag"), VertexShader("res/test.vert")});

  triangle["P"] =
      glm::ortho(0.0f, static_cast<float>(window.width), static_cast<float>(window.height), 0.0f);
  triangle["V"] = glm::mat4(1.0f);
  triangle["M"] =
      glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(200.0f)), glm::vec3(0.5f, 0.5f, 0.0f));

  triangleVerts = Buffer(std::vector<glm::vec3>{
      {+0.0f, -0.5f, 0.0f},
      {-0.5f, +0.5f, 0.0f},
      {+0.5f, +0.5f, 0.0f},
  });

  triangleColors = Buffer(std::vector<glm::vec3>{
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

  triangle.use();
  triangleVerts.setAttrib(triangle, "vertexPosition");
  triangleColors.setAttrib(triangle, "vertexColor");
  triangleVerts.drawArrays();

  texture.use();
  textureVerts.setAttrib(texture, "vertexPosition");
  textureUVs.setAttrib(texture, "vertexUV", true);
  textureVerts.drawArrays();
}
