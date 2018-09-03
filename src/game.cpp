#include "game.hpp"

Game::Game() : window(800, 600, false, 3, 0), running(true) {}

Game::~Game() {
  glfwTerminate();
}

void Game::run(int argc, char **argv) {
  init(argc, argv);
  while(this->running && !this->window.shouldClose()) {
    update();
    draw();
    this->window.swapBuffers();
    glfwPollEvents();
  }
}
