#include "buffer.hpp"


Buffer::Buffer() : Buffer(GL_ARRAY_BUFFER) {
}

Buffer::Buffer(GLenum target) : id(0), size(0), target(target) {

}

Buffer::Buffer(Buffer const &b) : id(b.id), size(b.size), target(b.target), _elemSize(b._elemSize) {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end())
        iterid->second = iterid->second + 1;
}

Buffer::~Buffer() {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            this->del();
    }
}

void Buffer::del() {
    glDeleteBuffers(1, &this->id);
}

Buffer &Buffer::operator=(Buffer const &b) {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteBuffers(1, &this->id);
    }
    this->id = b.id;
    this->size = b.size;
    this->target = b.target;
    this->_elemSize = b._elemSize;
    iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end())
        iterid->second = iterid->second + 1;
    return *this;
}

Buffer &Buffer::operator=(Buffer &&b) {
    std::swap(this->id, b.id);
    std::swap(this->size, b.size);
    std::swap(this->target, b.target);
    std::swap(this->_elemSize, b._elemSize);
    return *this;
}

Buffer::operator GLuint() {
    return this->id;
}


void Buffer::setElemSize(int elemSize) {
  _elemSize = elemSize;
}

template<>
void Buffer::setElemSize<glm::vec3>() {
  setElemSize(3);
}

template<>
void Buffer::setElemSize<glm::vec2>() {
  setElemSize(2);
}

template<>
void Buffer::setElemSize<float>() {
  setElemSize(1);
}

void Buffer::bind(GLenum target) const {
    glBindBuffer(target, this->id);
}

void Buffer::bind() const {
    bind(this->target);
}

void Buffer::unbind(GLenum target) const {
    glBindBuffer(this->target, 0);
}

void Buffer::unbind() const {
    unbind(this->target);
}

void Buffer::setAttrib(GLuint attribute,                                bool normalized, int divisor, GLenum type) const {
    glEnableVertexAttribArray(attribute);
    this->bind();
    glVertexAttribPointer(
        attribute,
        _elemSize,
        type,
        normalized?GL_TRUE:GL_FALSE,
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    if(divisor > 0)
      glVertexAttribDivisor(attribute, divisor);
}

void Buffer::setAttrib(const ShaderProgram &s, const std::string& name, bool normalized, int divisor, GLenum type) const {
    setAttrib(s, name.c_str(), normalized, divisor, type);
}

void Buffer::setAttrib(const ShaderProgram &s, const char *name,        bool normalized, int divisor, GLenum type) const {
    setAttrib(s.getAttribLocation(name), normalized, divisor, type);
}


void Buffer::drawArrays(GLenum mode) {
    glDrawArrays(mode, 0, this->size);
}

void Buffer::drawElements(GLenum mode, GLenum type) {
    this->bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElements(
        mode,
        this->size,
        type,
        (void*)0           // element array buffer offset
    );
}

void Buffer::drawInstanced(int count) const {
    drawInstanced(GL_TRIANGLES, count);
}

void Buffer::drawInstanced(GLenum mode, int count) const {
    drawInstanced(mode, count, GL_UNSIGNED_INT);
}

void Buffer::drawInstanced(GLenum mode, int count, GLenum type) const {
    this->bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElementsInstanced(
        mode,
        this->size,
        type,
        (void*)0,
        count
    );
}

IndexBuffer::IndexBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {
}

IndexBuffer::IndexBuffer(IndexBuffer const &b) : Buffer(b) {
}

IndexBuffer::IndexBuffer(std::vector<unsigned> const &data, GLenum usage) : Buffer(data, GL_ELEMENT_ARRAY_BUFFER, usage) {
}

IndexBuffer::IndexBuffer(std::vector<unsigned> const &data) : Buffer(data, GL_ELEMENT_ARRAY_BUFFER) {
}


std::map<GLuint, unsigned int> Buffer::refCount;
