#ifndef TEXT_H
#define TEXT_H

#include "buffer.hpp"
#include "shader.hpp"

class Text {
public:
  Text();
  Text(Text const &t);
  Text(std::string str);
  Text(std::string str, unsigned width, unsigned height);
  Text(unsigned width, unsigned height);
  Text &operator=(Text const &t);

  void setStr(std::string str);
  void draw();

private:
  void initProgram();
  void genStatic();
  void genBuffers();

  unsigned width = 80;
  unsigned height = 20;

  ShaderProgram p;
  std::string str;

  Buffer vertices;
  Buffer uvs;
  Buffer attrs;
  Buffer foreground;
  Buffer background;
};

#endif
