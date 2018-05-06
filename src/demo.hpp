#ifndef DEMO_H
#define DEMO_H

#include "game.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include "model.hpp"

class Demo : public Game {
  protected:
    virtual void init(int argc, char **argv);
    virtual void update();
    virtual void draw();

    ShaderProgram p;
    Buffer verts;
    Buffer colors;
};


#endif
