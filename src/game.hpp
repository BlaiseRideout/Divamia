#ifndef GAME_H
#define GAME_H

#include "window.hpp"

#include <glm/glm.hpp>

class Game {
public:
  Game();
  virtual ~Game();
  void run(int argc, char **argv);

protected:
  virtual void init(int argc, char **argv) = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  Window window;
  bool running;
};

#endif
