#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>
#include <sstream>
#include <algorithm>


Shader::Shader(std::string const &filename, GLenum type) {
  id = glCreateShader(type);
  load(filename);
  incRef();
}

Shader::Shader(Shader const &s) : id(s.id) {
  incRef();
}

Shader::Shader(Shader &&s) : id(0) {
  std::swap(id, s.id);
}

Shader::Shader() : id(0) {
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
  std::string shaderCode = this->loadCode(filename);

  // Compile Shader
  std::cout << "Compiling shader: " << filename << std::endl;
  char const *SourcePointer = shaderCode.c_str();
  glShaderSource(this->id, 1, &SourcePointer, NULL);
  glCompileShader(this->id);

  // Check Shader
  GLint result = GL_FALSE;
  glGetShaderiv(this->id, GL_COMPILE_STATUS, &result);

  if(result == GL_FALSE) {
    int InfoLogLength = 0;
    glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::string ShaderErrorMessage;
    ShaderErrorMessage.resize(InfoLogLength);
    glGetShaderInfoLog(this->id, InfoLogLength, NULL, &ShaderErrorMessage[0]);

    std::cerr << ShaderErrorMessage << std::endl;

    throw std::runtime_error(ShaderErrorMessage);
  }

}

std::string Shader::loadCode(std::string const &filename) {
  // Read the shader code from the file
  std::string shaderCode;
  std::ifstream shaderStream(filename.c_str(), std::ios::in);

  if(shaderStream.is_open()) {
    std::string line = "";
    while(getline(shaderStream, line))
      shaderCode += "\n" + line;
    shaderStream.close();
  }
  return shaderCode;
}

void Shader::del() {
  glDeleteShader(this->id);
  auto iterid = Shader::refCount.find(this->id);
  if(iterid != Shader::refCount.end())
    Shader::refCount.erase(iterid);
}

VertexShader::VertexShader(std::string const &filename)                 : Shader(filename, GL_VERTEX_SHADER) {}
FragmentShader::FragmentShader(std::string const &filename)             : Shader(filename, GL_FRAGMENT_SHADER) {}
GeometryShader::GeometryShader(std::string const &filename)             : Shader(filename, GL_GEOMETRY_SHADER) {}

#ifdef GL_VERSION_4_3
ComputeShader::ComputeShader(std::string const &filename)               : Shader(filename, GL_COMPUTE_SHADER) {}
#endif

TessControlShader::TessControlShader(std::string const &filename)       : Shader(filename, GL_TESS_CONTROL_SHADER) {}
TessEvaluationShader::TessEvaluationShader(std::string const &filename) : Shader(filename, GL_TESS_EVALUATION_SHADER) {}

void Shader::decRef() {
  if(id == 0)
    return;
  auto iterid = Shader::refCount.find(this->id);
  if(iterid != Shader::refCount.end()) {
    iterid->second = iterid->second - 1;
    if(iterid->second == 0)
      this->del();
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
    Shader::refCount.emplace(id, 1);
}

ShaderProgram::ShaderProgram() : id(0) {
}

ShaderProgram::ShaderProgram(ShaderProgram const &s) : id(s.id) {
  incRef();
}

ShaderProgram::ShaderProgram(ShaderProgram &&s) : id(0) {
  std::swap(id, s.id);
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> s) : shaders(s) {
  id = glCreateProgram();

  for (const Shader& shader : shaders)
    glAttachShader(id, shader.id);

  glLinkProgram(this->id);

  for (const Shader& shader : shaders)
    glDetachShader(id, shader.id);

  GLint Result = GL_FALSE;
  // Check the program
  glGetProgramiv(this->id, GL_LINK_STATUS, &Result);
  if(Result == GL_FALSE) {
      int InfoLogLength;
      glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
      std::string ProgramErrorMessage;
      ProgramErrorMessage.resize(std::max(InfoLogLength, 1));
      glGetProgramInfoLog(this->id, InfoLogLength, NULL, &ProgramErrorMessage[0]);

      throw std::runtime_error(ProgramErrorMessage);
  }

  incRef();
}

ShaderProgram::~ShaderProgram() {
  decRef();
}

void ShaderProgram::del() {
  glDeleteProgram(this->id);

  auto uid = ShaderProgram::_uids.find(this->id);
  if(uid != ShaderProgram::_uids.end())
    ShaderProgram::_uids.erase(uid);

  auto aid = ShaderProgram::_aids.find(this->id);
  if(aid != ShaderProgram::_aids.end())
    ShaderProgram::_aids.erase(aid);

  auto tex = ShaderProgram::gtextures.find(this->id);
  if(tex != ShaderProgram::gtextures.end())
    ShaderProgram::gtextures.erase(tex);

  auto iterid = ShaderProgram::refCount.find(this->id);
  if(iterid != ShaderProgram::refCount.end())
    ShaderProgram::refCount.erase(iterid);
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
  std::swap(this->shaders, s.shaders);
  return *this;
}

void ShaderProgram::setUniform(GLuint name, int value) const {
    glUseProgram(this->id);
    glUniform1i(name, value);
}

void ShaderProgram::setUniform(GLuint name, unsigned int value) const {
    glUseProgram(this->id);
    glUniform1ui(name, value);
}

void ShaderProgram::setUniform(GLuint name, float value) const {
    glUseProgram(this->id);
    glUniform1f(name, value);
}

void ShaderProgram::setUniform(GLuint name, glm::vec4 const &value) const {
    glUseProgram(this->id);
    glUniform4f(name, value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUniform(GLuint name, glm::vec3 const &value) const {
    glUseProgram(this->id);
    glUniform3f(name, value.x, value.y, value.z);
}

void ShaderProgram::setUniform(GLuint name, glm::vec2 const &value) const {
    glUseProgram(this->id);
    glUniform2f(name, value.x, value.y);
}

void ShaderProgram::setUniform(GLuint name, glm::mat4 const &value) const {
    glUseProgram(this->id);
    glUniformMatrix4fv(name, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(GLuint name, Texture const &val) const {
  glUseProgram(this->id);

  auto &textures = this->textures();
  unsigned index = textures.size();
  auto tex = std::find(textures.begin(), textures.end(), val);
  if(tex != textures.end())
    index = tex - textures.begin();
  else
    textures.push_back(val);

  glUniform1i(name, index);
}

Uniform ShaderProgram::operator[](std::string const &name) const {
    return Uniform(*this, getUniformLocation(name));
}

Uniform ShaderProgram::operator[](const char *name) const {
    return Uniform(*this, getUniformLocation(name));
}

GLuint ShaderProgram::getUniformLocation(std::string const &name) const {
  auto &uids = this->uids();

  auto iterid = uids.find(name);

  if(iterid == uids.end()) {
    GLint uid = glGetUniformLocation(id, name.c_str());
    if(uid == -1)
      throw std::runtime_error("Uniform not defined by shader: " + name);
    else
      iterid = uids.insert(uids.end(), std::pair<std::string, GLuint>(name, uid));
  }

  return iterid->second;
}

GLuint ShaderProgram::getAttribLocation(std::string const &name) const {
  auto &aids = this->aids();

  auto iterid = aids.find(name);
  if(iterid == aids.end()) {
    GLint aid = glGetAttribLocation(id, name.c_str());
    if(aid == -1)
      throw std::runtime_error("Attribute not defined by shader: " + name);
    else
      iterid = aids.insert(aids.end(), std::pair<std::string, GLuint>(name, aid));
  }

  return iterid->second;
}

void ShaderProgram::use() const {
  if(id == 0)
    return;
  glUseProgram(id);
  auto textures = this->textures();
  for(unsigned i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    textures[i].bind();
  }
}

std::vector<Texture> &ShaderProgram::textures() const {
  if(this->id == 0)
    throw std::runtime_error("Error: Shader program not loaded yet");

  return gtextures[id];
}

std::map<std::string, GLuint> &ShaderProgram::uids() const {
  if(this->id == 0)
    throw std::runtime_error("Error: Shader program not loaded yet");

  auto i = _uids.find(this->id);
  if(i == _uids.end())
    i = _uids.insert(_uids.end(), std::pair<GLuint, std::map<std::string, GLuint>>(this->id, std::map<std::string, GLuint>()));

  return i->second;
}

std::map<std::string, GLuint> &ShaderProgram::aids() const {
  if(this->id == 0)
    throw std::runtime_error("Error: Shader program not loaded yet");

  auto i = _aids.find(this->id);
  if(i == _aids.end())
    i = _aids.insert(_aids.end(), std::pair<GLuint, std::map<std::string, GLuint>>(this->id, std::map<std::string, GLuint>()));

  return i->second;
}
void ShaderProgram::decRef() {
  if(id == 0)
    return;
  auto iterid = ShaderProgram::refCount.find(this->id);
  if(iterid != ShaderProgram::refCount.end()) {
    iterid->second = iterid->second - 1;
    if(iterid->second == 0)
      this->del();
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

Uniform::Uniform(ShaderProgram const &program, GLuint id) : program(program), id(id) {
}

std::string Uniform::getName() {
    GLint size;
    GLenum type;
    GLchar name[256];
    glGetActiveUniform(program.id, id, 255, NULL, &size, &type, name);
    return std::string(name);
}

GLenum Uniform::getType() {
    GLint size;
    GLenum type;
    GLchar name;
    glGetActiveUniform(program.id, id, 0, NULL, &size, &type, &name);
    return type;
}

GLint Uniform::getSize() {
    GLint size;
    GLenum type;
    GLchar name;
    glGetActiveUniform(program.id, id, 0, NULL, &size, &type, &name);
    return size;
}

std::map<GLuint, unsigned> Shader::refCount;

std::map<GLuint, std::map<std::string, GLuint>> ShaderProgram::_uids;
std::map<GLuint, std::map<std::string, GLuint>> ShaderProgram::_aids;
std::map<GLuint, std::vector<Texture>> ShaderProgram::gtextures;
std::map<GLuint, unsigned> ShaderProgram::refCount;
