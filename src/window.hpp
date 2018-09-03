#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class Window {
public:
  Window(int width = 800,
         int height = 600,
         bool fullscreen = false,
         int majorVersion = 2,
         int minorVersion = 1,
         const std::string &title = "Divamia");
  Window(Window const &w);
  Window(Window &&w) = default;
  Window(GLFWwindow *window);
  virtual ~Window();
  operator GLFWwindow *() const;
  void clearColor(glm::vec3 color);
  void clearColor(glm::vec4 color);
  void clearScreen();
  void setViewport();
  void swapBuffers();
  void makeCurrent();
  bool shouldClose();
  void shouldClose(bool should);
  int getKey(int key) const;

  int width;
  int height;
  bool fullscreen;
  std::string title;
  int majorVersion;
  int minorVersion;

private:
  void initGL();
  void initGlfw();

  bool initialized = false;
  GLFWwindow *window;
  static std::map<GLFWwindow *, unsigned> refCount;
  static GLFWwindow *currentWindow;
};

#endif
