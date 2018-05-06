#include "window.hpp"

#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

Window::Window(int width, int height, bool fullscreen, int majorVersion, int minorVersion, const std::string& title) : width(width), height(height), fullscreen(fullscreen), title(title), majorVersion(majorVersion), minorVersion(minorVersion) {
  initGlfw();
  Window::refCount.insert(std::pair<GLFWwindow*, unsigned>(this->window, 1));
  initGL();
}

Window::Window(Window const &w) :
  width(w.width),
  height(w.height),
  fullscreen(w.fullscreen),
  title(w.title),
  majorVersion(w.majorVersion),
  minorVersion(w.minorVersion),
  window(w.window),
  initialized(w.initialized) {
  auto iterid = Window::refCount.find(this->window);
  if(iterid != Window::refCount.end())
      iterid->second = iterid->second + 1;
}

Window::Window(GLFWwindow *window) : window(window) {
  glfwGetWindowSize(this->window, &this->width, &this->height);
  fullscreen = glfwGetWindowMonitor(this->window) != nullptr;
  // TODO: get title
}

Window::~Window() {
  auto iterid = Window::refCount.find(this->window);
  if (iterid != Window::refCount.end()) {
    iterid->second = iterid->second - 1;
    if (iterid->second == 0) {
      if (Window::currentWindow == window)
        Window::currentWindow = nullptr;
      glfwDestroyWindow(window);
    }
  }
  else if (window != nullptr)
    glfwDestroyWindow(window);
}

Window::operator GLFWwindow*() const {
  return this->window;
}

void Window::clearColor(glm::vec4 color) {
  glfwMakeContextCurrent(this->window);
  glClearColor(color.r, color.g, color.b, color.a);
  glfwMakeContextCurrent(Window::currentWindow);
}

void Window::clearColor(glm::vec3 color) {
  clearColor(glm::vec4(color, 1.0f));
}

void Window::clearScreen() {
  if(currentWindow != this->window)
    glfwMakeContextCurrent(this->window);
  setViewport();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(currentWindow != this->window)
    glfwMakeContextCurrent(Window::currentWindow);
}

void Window::setViewport() {
  glViewport(0, 0, width, height);
}

void Window::swapBuffers() {
  glfwSwapBuffers(this->window);
}

void Window::makeCurrent() {
  Window::currentWindow = this->window;
  glfwMakeContextCurrent(this->window);
}

bool Window::shouldClose() {
  return glfwWindowShouldClose(this->window);
}

void Window::shouldClose(bool should) {
  glfwSetWindowShouldClose(this->window, should?GL_TRUE:GL_FALSE);
}

int Window::getKey(int key) const {
  return glfwGetKey(this->window, key);
}

void Window::initGL() {
  assert(!initialized);

  glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 0.0f);
  glViewport(0, 0, this->width, this->height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::initGlfw() {
  assert(!initialized);

  // Initialize glfw
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW.");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->minorVersion);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  if(this->fullscreen)
    this->window = glfwCreateWindow(this->width, this->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
  else
    this->window = glfwCreateWindow(this->width, this->height, title.c_str(), nullptr, nullptr);

  if(!this->window)
    throw std::runtime_error("Failed to open GLFW window.");

  glfwSwapInterval(static_cast<int>(true));

  this->makeCurrent();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize glad");
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
}

std::map<GLFWwindow*, unsigned> Window::refCount;
GLFWwindow *Window::currentWindow = nullptr;
