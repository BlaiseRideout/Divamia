#include "text.hpp"

#include <glm/gtc/matrix_transform.hpp>


Text::Text() : Text("", 20, 20) {
}

Text::Text(Text const &t) : width(t.width), height(t.height), str(t.str), vertices(t.vertices), uvs(t.uvs) {
}

Text::Text(std::string str) : Text(str, 20, 20) {
}

Text::Text(unsigned width, unsigned height) : Text("", width, height) {
}

Text::Text(std::string str, unsigned width, unsigned height) : width(width), height(height), str(str) {
	initProgram();
	genStatic();
	genBuffers();
}

void Text::initProgram() {
	this->p = ShaderProgram({FragmentShader("res/text.frag"), VertexShader("res/text.vert")});

	this->p["P"] = glm::ortho(0.0f, width * 1.0f, height * 1.0f, 0.0f);
	auto tex = Texture("res/font.png", GL_NEAREST);
  this->p["tex"] = tex;
	this->p["texsize"] = glm::vec2(tex.width(), tex.height());
}


void Text::setStr(std::string str) {
	this->str = str;
	genBuffers();
}

void Text::draw() {
	p.use();

	vertices.setAttrib(p, "vertexPosition");
	uvs.setAttrib(p, "vertexUV");

	vertices.drawArrays();
}

void Text::genStatic() {
	std::vector<glm::vec3> verts;
	const float size = 1.0f;
	for(unsigned i = 0; i < width * height; ++i) {
		const unsigned x = i % width, y = i / width;
		glm::vec3 vertex_up_left    = glm::vec3(x       , y + size, 0.0f);
		glm::vec3 vertex_up_right   = glm::vec3(x + size, y + size, 0.0f);
		glm::vec3 vertex_down_right = glm::vec3(x + size, y       , 0.0f);
		glm::vec3 vertex_down_left  = glm::vec3(x       , y       , 0.0f);

		verts.push_back(vertex_up_left);
		verts.push_back(vertex_up_right);
		verts.push_back(vertex_down_right);

		verts.push_back(vertex_down_right);
		verts.push_back(vertex_down_left);
		verts.push_back(vertex_up_left);
	}

	vertices = Buffer(verts);
}

void Text::genBuffers() {
	std::vector<glm::vec2> temp_uvs;

	const unsigned length = this->str.length();
	for(unsigned i = 0; i < length; ++i) {
		const char c = str[i];
		const float uv_x = (c % 16) / 16.0f;
		const float uv_y = (c / 16) / 16.0f;

		glm::vec2 uv_down_left  = glm::vec2(uv_x               , 1.0f - uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
		glm::vec2 uv_up_right   = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_up_left    = glm::vec2(uv_x               , 1.0f - (uv_y + 1.0f / 16.0f));

		temp_uvs.push_back(uv_up_left);
		temp_uvs.push_back(uv_up_right);
		temp_uvs.push_back(uv_down_right);

		temp_uvs.push_back(uv_down_right);
		temp_uvs.push_back(uv_down_left);
		temp_uvs.push_back(uv_up_left);
	}

	uvs = Buffer(temp_uvs);
}

Text &Text::operator=(Text const &t) {
	p = t.p;
	str = t.str;
	width = t.width;
	height = t.height;
	vertices = t.vertices;
	uvs = t.uvs;

	return *this;
}
