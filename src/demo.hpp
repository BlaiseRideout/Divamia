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
	public:
		Demo();
		virtual ~Demo();
	protected:
		virtual void init(int argc, char **argv);
		virtual void update();
		virtual void draw();

		Texture t;
		ShaderProgram p;
		//Buffer inds;
		//VAO vao;
		//Model m;
		//FPCamera c;

};


#endif
