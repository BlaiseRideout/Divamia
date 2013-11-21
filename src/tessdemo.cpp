#include "tessdemo.hpp"

#include <glm/gtc/matrix_transform.hpp>


TessDemo::TessDemo() : Game("Tesselation Demo", 4, 2), c(window) {
	initShader();
	initBuffers();
}

void TessDemo::initShader() {
	VertexShader v("res/tess.vert");
	TessControlShader tc("res/tess.tctrl");
	TessEvaluationShader te("res/tess.teval");
	GeometryShader g("res/tess.geom");
	FragmentShader f("res/tess.frag");

	this->p = ShaderProgram(v, tc, te, g, f);

	glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 V = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));

	this->p["M"] = M;
	this->p["V"] = V;
	this->p["P"] = glm::perspective(45.0f, (float)this->window.width / (float)this->window.height, 0.1f, 10.0f);
	//this->p["P"] = glm::mat4(1.0f);
	this->p["NormalMatrix"] = glm::mat3(glm::transpose(glm::inverse(V * M)));

	this->p["LightPosition"] = glm::vec4(-0.5f, 1.0f, 1.0f, 0.0f);
	this->p["DiffuseMaterial"] = glm::vec3(0.7f, 0.7f, 1.0f);
	this->p["AmbientMaterial"] = glm::vec3(0.2f, 0.2f, 0.2f);

	this->p["TessLevelInner"] = 3.0f;
	this->p["TessLevelOuter"] = 2.0f;

	/*this->p = ShaderProgram(VertexShader("res/texture.vert"), FragmentShader("res/color.frag"));

	this->p["M"] = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	this->p["V"] = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
	this->p["P"] = glm::perspective(45.0f, (float)this->window.width / (float)this->window.height, 0.1f, 10.0f);

	this->p["color"] = glm::vec4(0.7f, 0.7f, 1.0f, 1.0f); */
}

void TessDemo::initBuffers() {
	std::vector<glm::vec3> verts({
		glm::vec3(0.000f,  0.000f,  1.000f),
		glm::vec3(0.894f,  0.000f,  0.447f),
		glm::vec3(0.276f,  0.851f,  0.447f),
		glm::vec3(-0.724f,  0.526f,  0.447f),
		glm::vec3(-0.724f, -0.526f,  0.447f),
		glm::vec3(0.276f, -0.851f,  0.447f),
		glm::vec3(0.724f,  0.526f, -0.447f),
		glm::vec3(-0.276f,  0.851f, -0.447f),
		glm::vec3(-0.894f,  0.000f, -0.447f),
		glm::vec3(-0.276f, -0.851f, -0.447f),
		glm::vec3(0.724f, -0.526f, -0.447f),
		glm::vec3(0.000f,  0.000f, -1.000f)
	});

	std::vector<unsigned> faces({
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,

		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,

		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,

		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10
    });

	Buffer positions(verts);

	this->vertices.setAttrib(this->p, "Position", positions, 3);

	this->indices = IndexBuffer(faces);
	glPointSize(10);
}

void TessDemo::update(double time) {
	running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;

	this->c.update();
	this->p["V"] = this->c.viewMatrix();

	this->window.clearScreen();

	vertices.bind();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	indices.drawElements(GL_PATCHES);
	//indices.drawElements();

}

TessDemo::~TessDemo() {
}