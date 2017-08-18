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
	this->p = ShaderProgram(FragmentShader("res/text.frag"), VertexShader("res/text.vert"));

	this->p["P"] = glm::ortho(0.0f, width * 1.0f, height * 1.0f, 0.0f, -0.1f, 1.0f);
  this->p["tex"] = Texture("res/font.png", GL_NEAREST);
	this->p["color"] = glm::vec3(1.0f);

	genBuffers();
}

void Text::setStr(std::string str) {
	this->str = str;
	genBuffers();
}

void Text::draw() {
	this->p.tempUse();
	this->vertices.drawArrays();
	this->p.useCurrent();
}

void Text::genBuffers() {
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> temp_uvs;

	unsigned curx = 0, cury = 0;
	float size = 1.0f;
	float ysize = 1.0f;
	for(unsigned i = 0; i < this->str.length(); ++i) {
		glm::vec3 vertex_up_left    = glm::vec3(curx       , cury + ysize, 0.0f);
		glm::vec3 vertex_up_right   = glm::vec3(curx + size, cury + ysize, 0.0f);
		glm::vec3 vertex_down_right = glm::vec3(curx + size, cury        , 0.0f);
		glm::vec3 vertex_down_left  = glm::vec3(curx       , cury        , 0.0f);

		++curx;
		if(curx == width) {
			++cury;
			curx = 0;
		}
		if(cury == height)
			break;

		verts.push_back(vertex_up_left);
		verts.push_back(vertex_up_right);
		verts.push_back(vertex_down_right);

		verts.push_back(vertex_down_right);
		verts.push_back(vertex_down_left);
		verts.push_back(vertex_up_left);

		char c = str[i];
		float uv_x = (c % 16) / 16.0f;
		float uv_y = (c / 16) / 16.0f;

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

	vertices = Buffer(verts);
	uvs = Buffer(temp_uvs);

	vertices.setAttrib(this->p, "vertexPosition", 3, GL_FLOAT, false);
	uvs.setAttrib(this->p, "vertexUV", 2, GL_FLOAT, false);
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
