#pragma once

#include "texture.hpp"
#include "window.hpp"

class Framebuffer {
	public:
		Framebuffer(unsigned width, unsigned height);
		Framebuffer(unsigned width, unsigned height, GLint attachment);
		~Framebuffer();
		void bind() const;
		void unbind() const;
		operator GLuint() const;
		operator Texture() const;

		Texture tex;
		GLenum attachment = GL_COLOR_ATTACHMENT0;
	protected:
		unsigned width, height;
		GLuint id = 0;
};
