#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

#include "ellipse.hpp"

int main(int argc, char **argv) {
  try {
    Ellipse game;
    game.run(argc, argv);
    return 0;
  }
  catch(const std::exception &exc) {
    std::cerr << exc.what() << std::endl;
  }
}
