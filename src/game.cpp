#include "game.hpp"


Game::Game(std::string title, int majorVersion, int minorVersion) : window(800, 600, false, title, majorVersion, minorVersion) {
  this->running = true;
}

Game::Game() : Game("Divamia", 3, 3) {
>>>>>>> d6f58c3e1fec9ee471b0f173a5b1c620acdc53ec
}

Game::Game(std::string title) : Game(title, 3, 3) {
}

Game::Game() : Game("Divamia") {
}

Game::~Game() {
  glfwTerminate();
}

void Game::run() {
  while(this->running && !this->window.shouldClose()) {
    double oldTime = this->time;
    this->time = glfwGetTime();
    update(this->time - oldTime);
    this->window.swapBuffers();
    glfwPollEvents();
  }
}
