#include "framebuffer.hpp"

#include <cassert>
#include <iostream>

Framebuffer::Framebuffer(unsigned width, unsigned height) : Framebuffer(width, height, GL_COLOR_ATTACHMENT0) {
}

Framebuffer::Framebuffer(unsigned width, unsigned height, GLint attachment) : tex(width, height, GL_NEAREST, GL_NEAREST), attachment(attachment), width(width), height(height), id(0) {
	glGenFramebuffers(1, &id);
	glBindTexture(GL_TEXTURE_2D, 0);

	bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	std::cout << "Framebuffer texture: " << tex << "(" << tex.width() << "x" << tex.height() << ")" << std::endl;

	unbind();
}

Framebuffer::~Framebuffer() {
	if(id != 0) {
		glDeleteFramebuffers(1, &id);
		id = 0;
	}
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, width, height);
}

void Framebuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::operator GLuint() const {
	return id;
}

Framebuffer::operator Texture() const {
	return tex;
}
