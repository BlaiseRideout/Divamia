#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <map>

#include "image.hpp"

class Texture {
	public:
		Texture();
		Texture(GLint filter);
		Texture(GLint mag_filter, GLint min_filter);
		Texture(GLuint id);
		Texture(Texture const &t);
		Texture(Texture &&t);

		Texture(std::string name);
		Texture(std::string name, GLint filter);
		Texture(std::string name, GLint mag_filter, GLint min_filter);

		Texture(Image const &img);
		Texture(Image const &img, GLint filter);
		Texture(Image const &img, GLint mag_filter, GLint min_filter);

		~Texture();
		Texture &operator=(Texture const &s);
		Texture &operator=(Texture &&s);
		Texture &operator=(Image const &i);
		bool operator==(Texture const &s);

		unsigned int width();
		unsigned int height();
		void bind() const;
	protected:
		GLuint id;
		GLint mag_filter;
		GLint min_filter;

		static void decRef(GLuint id);
		static void incRef(GLuint id);

		static GLuint currentTexture;
		static std::map<GLuint, unsigned int> refCount;
};

#endif
