#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>
#include <sstream>
#include <algorithm>

#include "shader.hpp"

Shader::Shader(std::string const &filename, GLenum type) {
	this->id = glCreateShader(type);
	this->load(filename);
	incRef();
}

Shader::Shader(Shader const &s) : id(s.id) {
	incRef();
}

Shader::Shader(Shader &&s) {
	std::swap(id, s.id);
}

Shader::Shader() {
}

Shader::~Shader() {
	decRef();
}

Shader &Shader::operator=(Shader const &s) {
	decRef();
	this->id = s.id;
	incRef();
	return *this;
}

Shader &Shader::operator=(Shader &&s) {
	std::swap(s.id, this->id);
	return *this;
}

void Shader::load(std::string const &filename) {
    std::string ShaderCode = this->loadCode(filename);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Shader
    std::cout << "Compiling shader : " << filename << std::endl;
    char const *SourcePointer = ShaderCode.c_str();
    glShaderSource(this->id, 1, &SourcePointer , NULL);
    glCompileShader(this->id);

    // Check Shader
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &Result);
    if(Result == GL_FALSE) {
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::string ShaderErrorMessage;
        ShaderErrorMessage.resize(InfoLogLength);
        glGetShaderInfoLog(this->id, InfoLogLength, NULL, &ShaderErrorMessage[0]);

        throw std::runtime_error(ShaderErrorMessage);
    }
}

std::string Shader::loadCode(std::string const &filename) {
    // Read the shader code from the file
    std::string ShaderCode;
    std::ifstream ShaderStream(filename.c_str(), std::ios::in);

    if(ShaderStream.is_open()){
        std::string Line = "";
        while(getline(ShaderStream, Line))
            ShaderCode += "\n" + Line;
        ShaderStream.close();
    }
    return ShaderCode;
}

void Shader::del() {
	glDeleteShader(this->id);
}

VertexShader::VertexShader(std::string const &filename) : Shader(filename, GL_VERTEX_SHADER) {
}

FragmentShader::FragmentShader(std::string const &filename) : Shader(filename, GL_FRAGMENT_SHADER) {
}

GeometryShader::GeometryShader(std::string const &filename) : Shader(filename, GL_GEOMETRY_SHADER) {
}

#ifdef GL_VERSION_4_3
ComputeShader::ComputeShader(std::string const &filename) : Shader(filename, GL_COMPUTE_SHADER) {
}
#endif

TessControlShader::TessControlShader(std::string const &filename) : Shader(filename, GL_TESS_CONTROL_SHADER) {
}

TessEvaluationShader::TessEvaluationShader(std::string const &filename) : Shader(filename, GL_TESS_EVALUATION_SHADER) {
}

void Shader::decRef() {
	if(id == 0)
		return;
	auto iterid = Shader::refCount.find(this->id);
	if(iterid != Shader::refCount.end()) {
		iterid->second = iterid->second - 1;
		if(iterid->second == 0) {
			this->del();
			Shader::refCount.erase(iterid);
		}
	}
	else
		std::cerr << "Asked to decrement untracked shader " << id << "!" << std::endl;
}

void Shader::incRef() {
	if(id == 0)
		return;
	auto iterid = Shader::refCount.find(id);
	if(iterid != Shader::refCount.end())
			iterid->second = iterid->second + 1;
	else
		Shader::refCount.insert(std::pair<GLint, int>(id, 1));
}

ShaderProgram::ShaderProgram() : id(0) {
}

ShaderProgram::ShaderProgram(ShaderProgram const &s) : id(s.id), shaders(s.shaders) {
	incRef();
}

ShaderProgram::ShaderProgram(ShaderProgram &&s) {
	std::swap(id, s.id);
	std::swap(shaders, s.shaders);
	std::swap(uids, s.uids);
	std::swap(aids, s.aids);
}

ShaderProgram::~ShaderProgram() {
	decRef();
}

void ShaderProgram::del() {
	glDeleteProgram(this->id);

	auto iterid = ShaderProgram::refCount.find(this->id);
	if(iterid != ShaderProgram::refCount.end())
		ShaderProgram::refCount.erase(iterid);

	auto i = ShaderProgram::gtextures.find(this->id);
	if(i != ShaderProgram::gtextures.end())
		ShaderProgram::gtextures.erase(i);
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram const &s) {
	decRef();
	this->id = s.id;
	this->shaders = s.shaders;
	incRef();
	return *this;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&s) {
	std::swap(this->id, s.id);
	std::swap(shaders, s.shaders);
	std::swap(uids, s.uids);
	std::swap(aids, s.aids);
	return *this;
}

ShaderProgram::operator GLuint() {
	return this->id;
}

void ShaderProgram::setUniform(std::string const &name, int value) {
    glUseProgram(this->id);
    glUniform1i(this->getUniformLocation(name), value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, unsigned int value) {
    glUseProgram(this->id);
    glUniform1ui(this->getUniformLocation(name), value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, float value) {
    glUseProgram(this->id);
    glUniform1f(this->getUniformLocation(name), value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, glm::vec4 const &value) {
    glUseProgram(this->id);
    glUniform4f(this->getUniformLocation(name), value.x, value.y, value.z, value.w);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, glm::vec3 const &value) {
    glUseProgram(this->id);
    glUniform3f(this->getUniformLocation(name), value.x, value.y, value.z);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, glm::vec2 const &value) {
    glUseProgram(this->id);
    glUniform2f(this->getUniformLocation(name), value.x, value.y);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, glm::mat4 const &value) {
    glUseProgram(this->id);
    glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string const &name, Texture const &val) {
	glUseProgram(this->id);

	std::vector<Texture> &textures = this->textures();
	unsigned index = textures.size();
	auto tex = std::find(textures.begin(), textures.end(), val);
	if(tex != textures.end())
		index = tex - textures.begin();
	else
		textures.push_back(val);

	glUniform1i(this->getUniformLocation(name), index);
	glActiveTexture(GL_TEXTURE0 + index);
	textures[index].bind();

	if(ShaderProgram::currentProgram != this->id) {
		glUseProgram(ShaderProgram::currentProgram);

		auto i = ShaderProgram::gtextures.find(ShaderProgram::currentProgram);
		if(i != ShaderProgram::gtextures.end()) {
			std::vector<Texture> &textures = i->second;
			if(textures.size() > index) {
				glActiveTexture(GL_TEXTURE0 + index);
				textures[index].bind();
			}
		}
	}
}

Uniform ShaderProgram::operator[](std::string const &name) {
    return Uniform(this->id, getUniformLocation(name));
}

Uniform ShaderProgram::operator[](const char *name) {
    return Uniform(this->id, getUniformLocation(std::string(name)));
}

GLuint ShaderProgram::getUniformLocation(std::string const &name) {
    auto iterid = this->uids.find(name);
    if(iterid != this->uids.end())
        return iterid->second;
    else {
        GLint uid = glGetUniformLocation(this->id, name.c_str());
        if(uid == -1)
            throw std::runtime_error("Uniform not defined by shader: " + name);
        else
            this->uids.insert(std::pair<std::string, GLuint>(name, uid));
        return uid;
    }
}

GLuint ShaderProgram::getAttribLocation(std::string const &name) {
    auto iterid = this->aids.find(name);
    if(iterid != this->aids.end())
        return iterid->second;
    else {
        GLint aid = glGetAttribLocation(this->id, name.c_str());
        if(aid == -1)
            throw std::runtime_error("Attribute not defined by shader: " + name);
        else
            this->aids.insert(std::pair<std::string, GLuint>(name, aid));
        return aid;
    }
}

bool ShaderProgram::isSet(std::string name) {
    auto iterid = this->uids.find(name);
    return iterid != this->uids.end();
}

void ShaderProgram::use() {
    ShaderProgram::currentProgram = this->id;
		ShaderProgram::useShader(this->id);
}

void ShaderProgram::tempUse() {
		ShaderProgram::useShader(this->id);
}

void ShaderProgram::useCurrent() {
	if(ShaderProgram::currentProgram != 0)
		ShaderProgram::useShader(ShaderProgram::currentProgram);
}

void ShaderProgram::useShader(GLuint shader) {
	if(shader == 0)
		return;
	glUseProgram(shader);
	auto _textures = gtextures.find(shader);
	if(_textures != ShaderProgram::gtextures.end()) {
		auto textures = _textures->second;
		for(unsigned i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			textures[i].bind();
		}
	}
}

std::vector<Texture> &ShaderProgram::textures() {
	if(this->id == 0)
		throw std::runtime_error("Error: Shader program not loaded yet");

	auto i = ShaderProgram::gtextures.find(this->id);
	if(i != ShaderProgram::gtextures.end())
			return i->second;
	else {
			ShaderProgram::gtextures.insert(std::pair<GLuint, std::vector<Texture>>(this->id, std::vector<Texture>()));
			return this->textures();
	}
}

void ShaderProgram::decRef() {
	if(id == 0)
		return;
	auto iterid = ShaderProgram::refCount.find(this->id);
	if(iterid != ShaderProgram::refCount.end()) {
		iterid->second = iterid->second - 1;
		if(iterid->second == 0) {
			this->del();
		}
	}
	else
		std::cerr << "Asked to decrement untracked shaderprogram " << id << "!" << std::endl;
}

void ShaderProgram::incRef() {
	if(id == 0)
		return;
	auto iterid = ShaderProgram::refCount.find(this->id);
	if(iterid != ShaderProgram::refCount.end())
			iterid->second = iterid->second + 1;
	else
		ShaderProgram::refCount.insert(std::pair<GLint, int>(this->id, 1));
}

Uniform::Uniform(GLuint program, GLuint id) : program(program), id(id) {
}

Uniform &Uniform::operator=(int val) {
    glUseProgram(this->program);
    glUniform1i(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(unsigned int val) {
    glUseProgram(this->program);
    glUniform1ui(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(float val) {
    glUseProgram(this->program);
    glUniform1f(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec4 const &val) {
    glUseProgram(this->program);
    glUniform4f(this->id, val.x, val.y, val.z, val.w);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec3 const &val) {
    glUseProgram(this->program);
    glUniform3f(this->id, val.x, val.y, val.z);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec2 const &val) {
    glUseProgram(this->program);
    glUniform2f(this->id, val.x, val.y);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::mat4 const &val) {
    glUseProgram(this->program);
    glUniformMatrix4fv(this->id, 1, GL_FALSE, &val[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(Texture const &val) {
	glUseProgram(this->program);

	unsigned index = 0;
	auto i = ShaderProgram::gtextures.find(this->program);
	if(i != ShaderProgram::gtextures.end()) {
		std::vector<Texture> &textures = i->second;
		auto tex = std::find(textures.begin(), textures.end(), val);
		if(tex != textures.end())
			index = tex - textures.begin();
		else {
			index = textures.size();

			textures.push_back(val);
		}
		glUniform1i(this->id, index);

		glActiveTexture(GL_TEXTURE0 + index);
		textures[index].bind();
	}

	if(ShaderProgram::currentProgram != this->program) {
		glUseProgram(ShaderProgram::currentProgram);

		i = ShaderProgram::gtextures.find(ShaderProgram::currentProgram);
		if(i != ShaderProgram::gtextures.end()) {
			std::vector<Texture> &textures = i->second;
			if(textures.size() > index) {
				glActiveTexture(GL_TEXTURE0 + index);
				textures[index].bind();
			}
		}
	}

	return *this;
}

std::string Uniform::getName() {
    GLint size;
    GLenum type;
    GLchar name[256];
    glGetActiveUniform(program, id, 255, NULL, &size, &type, name);
    return std::string(name);
}

GLenum Uniform::getType() {
    GLint size;
    GLenum type;
    GLchar name;
    glGetActiveUniform(program, id, 0, NULL, &size, &type, &name);
    return type;
}

GLint Uniform::getSize() {
    GLint size;
    GLenum type;
    GLchar name;
    glGetActiveUniform(program, id, 0, NULL, &size, &type, &name);
    return size;
}

GLuint ShaderProgram::currentProgram = 0;
std::map<GLuint, unsigned> Shader::refCount;
std::map<GLuint, unsigned> ShaderProgram::refCount;
std::map<GLuint, std::vector<Texture>> ShaderProgram::gtextures;
