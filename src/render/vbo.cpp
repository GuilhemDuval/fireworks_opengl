#include "vbo.hpp"
#include <iostream>

VBO::VBO() {
  glGenBuffers(1, &id);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "Error generating VBO" << std::endl;
  }
}

VBO::~VBO() { glDeleteBuffers(1, &id); }

void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VBO::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::fill(const void *data, GLsizei size, GLenum usage) {
  bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "Error filling VBO" << std::endl;
  }
}

void VBO::update(const void *data, GLsizei size, GLintptr offset) {
  bind();
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "Error updating VBO" << std::endl;
  }
}

GLuint VBO::get_id() const { return id; }