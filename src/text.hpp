#ifndef TEXT_H
#define TEXT_H

#include "shader.hpp"
#include "buffer.hpp"

class Text {
	public:
		Text();
		Text(Text const &t);
		Text(std::string str);
		Text(std::string str, unsigned width, unsigned height);
		Text(unsigned width, unsigned height);
		Text &operator=(Text const &t);

		void setStr(std::string str);
		void drawStr(std::string str);
		void draw();

		unsigned width = 80;
		unsigned height = 20;
	private:
		void genBuffers();

		ShaderProgram p;
		std::string str;

		Buffer vertices;
		Buffer uvs;
};

#endif
