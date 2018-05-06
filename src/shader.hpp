#ifndef SHADER_H
#define SHADER_H

#include "texture.hpp"

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <initializer_list>

class Shader;
class VertexShader;
class FragmentShader;
class ShaderProgram;
class Uniform;

class Shader {
  public:
    Shader();
    Shader(std::string const &filename, GLenum type);
    Shader(Shader const &s);
    Shader(Shader &&s);
    virtual ~Shader();

    Shader &operator=(Shader const &s);
    Shader &operator=(Shader &&s);
    operator GLuint() const { return id; }
  protected:
    virtual void load(std::string const &filename);
    std::string loadCode(std::string const &filename);
    void del();
    void decRef();
    void incRef();

    GLuint id = 0;

    static std::map<GLuint, unsigned int> refCount;
    friend class ShaderProgram;
};

class VertexShader : public Shader {
  public:
    VertexShader(std::string const &filename);
};

class FragmentShader : public Shader {
  public:
    FragmentShader(std::string const &filename);
};

class GeometryShader : public Shader {
  public:
    GeometryShader(std::string const &filename);
};

#ifdef GL_VERSION_4_3
class ComputeShader : public Shader {
  public:
    ComputeShader(std::string const &filename);
};
#endif

class TessControlShader : public Shader {
  public:
    TessControlShader(std::string const &filename);
};

class TessEvaluationShader : public Shader {
  public:
    TessEvaluationShader(std::string const &filename);
};

class ShaderProgram {
  public:
    ShaderProgram();
    ShaderProgram(ShaderProgram const &s);
    ShaderProgram(ShaderProgram &&s);
    ShaderProgram(std::initializer_list<Shader> shaders);
    ~ShaderProgram();

    ShaderProgram &operator=(ShaderProgram const &s);
    ShaderProgram &operator=(ShaderProgram &&s);
    operator GLuint() const;

    void setUniform(GLuint name, int value) const;
    void setUniform(GLuint name, unsigned int value) const;
    void setUniform(GLuint name, float value) const;
    void setUniform(GLuint name, glm::vec4 const &value) const;
    void setUniform(GLuint name, glm::vec3 const &value) const;
    void setUniform(GLuint name, glm::vec2 const &value) const;
    void setUniform(GLuint name, glm::mat4 const &value) const;
    void setUniform(GLuint name, Texture const &value) const;
    template<typename T>
    void setUniform(std::string const &name, T &&val) const {
      setUniform(getUniformLocation(name), std::forward<T>(val));
    }

    GLuint getUniformLocation(std::string const &name) const;
    GLuint getAttribLocation(std::string const &name) const;

    Uniform operator[](std::string const &name) const;
    Uniform operator[](const char *name) const;
    void use() const;
  protected:
    void del();
    std::vector<Texture> &textures() const;
    std::map<std::string, GLuint> &uids() const;
    std::map<std::string, GLuint> &aids() const;
    void incRef();
    void decRef();

    GLuint id;
    std::vector<Shader> shaders;
    static std::map<GLuint, std::map<std::string, GLuint>> _uids;
    static std::map<GLuint, std::map<std::string, GLuint>> _aids;
    static std::map<GLuint, std::vector<Texture>> gtextures;
    static std::map<GLuint, unsigned> refCount;
  friend class Uniform;
};

class Uniform {
  public:
    template<typename T>
    Uniform &operator=(T const &val) {
      program.setUniform(id, val);
      return *this;
    }
    template<typename T>
    Uniform &operator=(T &&val) {
      program.setUniform(id, val);
      return *this;
    }

    std::string getName();
    GLenum getType();
    GLint getSize();
  protected:
    Uniform(ShaderProgram const &program, GLuint id);
    ShaderProgram const &program;
    GLuint id;
  friend class ShaderProgram;
};

#endif
