#include "game.hpp"


Game::Game() : window(800, 600, false, 2, 0), running(true) {
}

Game::~Game() {
  glfwTerminate();
}

void Game::run() {
  while(this->running && !this->window.shouldClose()) {
    update();
    draw();
    this->window.swapBuffers();
    glfwPollEvents();
  }
}
