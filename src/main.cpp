#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>

#include "demo.hpp"

int main(int argc, char **argv) {
  try {
    Demo game;
    game.run(argc, argv);
    return 0;
  }
  catch(const std::exception &exc) {
    std::cerr << exc.what() << std::endl;
  }
}
