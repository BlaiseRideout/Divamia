#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

#include "tessdemo.hpp"

int main(int argc, char **argv) {
  try {
    TessDemo game;
    game.run();
    return 0;
  }
  catch(const std::exception &exc) {
    std::cerr << exc.what() << std::endl;
  }
}