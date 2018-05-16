#ifndef DEMO_H
#define DEMO_H

#include "game.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "buffer.hpp"

class Demo : public Game {
  protected:
    virtual void init(int argc, char **argv);
    virtual void update();
    virtual void draw();

    void initTriangle();
    void initTexture();

    ShaderProgram triangle;

    ShaderProgram texture;

    Buffer triangleVerts;
    Buffer triangleColors;

    Buffer textureVerts;
    Buffer textureUVs;
};


#endif
