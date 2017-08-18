#include "texture.hpp"

#include <FreeImage.h>
#include <vector>
#include <stdexcept>
#include <iostream>

unsigned int Texture::width() {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint width;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &width);
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
	return width;
}

unsigned int Texture::height() {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
	return height;
}

void Texture::bind() const {
	Texture::currentTexture = this->id;
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
}

Texture::Texture() : id(0), mag_filter(GL_LINEAR), min_filter(GL_LINEAR) {
}

Texture::Texture(GLint filter) : id(0), mag_filter(filter), min_filter(filter) {
}

Texture::Texture(GLint mag_filter, GLint min_filter) : id(0), mag_filter(mag_filter), min_filter(min_filter) {
}

Texture::Texture(GLuint id) : id(id), mag_filter(GL_LINEAR), min_filter(GL_LINEAR) {
	incRef(this->id);
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

	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
}

Texture::~Texture() {
	decRef(this->id);
}

Texture &Texture::operator=(Texture const &s) {
	decRef(this->id);
	this->id = s.id;
	incRef(this->id);
	return *this;
}

Texture &Texture::operator=(Texture &&s) {
	std::swap(s.id, this->id);
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

	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);

	return *this;
}

bool Texture::operator==(const Texture &s) {
	return this->id == s.id;
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

GLuint Texture::currentTexture;
std::map<GLuint, unsigned int> Texture::refCount;
