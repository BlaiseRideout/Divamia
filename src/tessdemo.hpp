#ifndef TESS_DEMO_H
#define TESS_DEMO_H

#include "game.hpp"
#include "vao.hpp"
#include "shader.hpp"

class TessDemo : public Game {
	public:
		TessDemo();
		virtual ~TessDemo();
	protected:
		virtual void update(double time);
	private:
		void initShader();
		void initBuffers();

		ShaderProgram p;

		VAO	vertices;
		IndexBuffer indices;

		FPCamera c;
};

#endif