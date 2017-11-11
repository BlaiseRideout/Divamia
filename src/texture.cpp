#include "texture.hpp"

#include <FreeImage.h>
#include <vector>
#include <stdexcept>
#include <iostream>

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}

Texture::Texture() : id(0), mag_filter(GL_LINEAR), min_filter(GL_LINEAR) {
}

Texture::Texture(GLint mag_filter, GLint min_filter) : id(0), mag_filter(mag_filter), min_filter(min_filter) {
}

Texture::Texture(GLuint id) : id(id), mag_filter(GL_LINEAR), min_filter(GL_LINEAR) {
	incRef(this->id);
}

Texture::Texture(GLuint id, GLint mag_filter, GLint min_filter) : id(id), mag_filter(mag_filter), min_filter(min_filter) {
	incRef(this->id);

	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::Texture(Texture const &t) : id(t.id), mag_filter(t.mag_filter), min_filter(t.min_filter) {
	incRef(this->id);
}

Texture::Texture(Texture &&t) {
	std::swap(id, t.id);
	std::swap(min_filter, t.min_filter);
	std::swap(mag_filter, t.mag_filter);
}

Texture::Texture(std::string filename) : Texture(filename, GL_NEAREST) {
}

Texture::Texture(std::string filename, GLint filter) : Texture(filename, filter, filter) {
}

Texture::Texture(std::string filename, GLint mag_filter, GLint min_filter) : Texture(Image(filename), mag_filter, min_filter){
}

Texture::Texture(Image const &img) : Texture(img, GL_NEAREST) {
}

Texture::Texture(Image const &img, GLint filter) : Texture(img, filter, filter) {
}

Texture::Texture(Image const &img, GLint mag_filter, GLint min_filter) : mag_filter(mag_filter), min_filter(min_filter) {
	glGenTextures(1, &this->id);
	incRef(this->id);

	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.pix.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::Texture(unsigned width, unsigned height, GLint mag_filter, GLint min_filter) : mag_filter(mag_filter), min_filter(min_filter) {
	glGenTextures(1, &this->id);
	incRef(this->id);

	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

Texture::~Texture() {
	decRef(this->id);
}

Texture &Texture::operator=(Texture const &s) {
	decRef(this->id);
	this->id = s.id;
	min_filter = s.min_filter;
	mag_filter = s.mag_filter;
	incRef(this->id);
	return *this;
}

Texture &Texture::operator=(Texture &&s) {
	std::swap(s.id, this->id);
	std::swap(s.min_filter, s.min_filter);
	std::swap(s.mag_filter, s.mag_filter);
	return *this;
}

Texture &Texture::operator=(Image const &img) {
	if(!this->id) {
		glGenTextures(1, &this->id);
		incRef(this->id);
	}
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.pix.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	return *this;
}

bool Texture::operator==(const Texture &s) {
	return this->id == s.id;
}

Texture::operator GLuint() const {
	return id;
}

unsigned int Texture::width() const {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint width;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &width);
	return width;
}

unsigned int Texture::height() const {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	return height;
}

void Texture::decRef(GLuint id) {
	auto iterid = Texture::refCount.find(id);
	if(iterid != Texture::refCount.end()) {
			iterid->second = iterid->second - 1;
			if(iterid->second == 0) {
					glDeleteTextures(1, &id);
					Texture::refCount.erase(iterid);
			}
	}
	else if(id) {
		std::cerr << "Asked to decrement untracked texture!" << std::endl;
	}
}

void Texture::incRef(GLuint id) {
	auto iterid = Texture::refCount.find(id);
	if(iterid != Texture::refCount.end())
			iterid->second = iterid->second + 1;
	else
		Texture::refCount.insert(std::make_pair<GLuint, unsigned int>(std::move(id), 1));
}

std::map<GLuint, unsigned int> Texture::refCount;
