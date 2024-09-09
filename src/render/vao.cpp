#include "vao.hpp"
#include <iostream>

VAO::VAO() {
  glGenVertexArrays(1, &id);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "Error generating VAO" << std::endl;
  }
}

VAO::~VAO() { glDeleteVertexArrays(1, &id); }

void VAO::bind() const { glBindVertexArray(id); }

void VAO::unbind() const { glBindVertexArray(0); }

void VAO::specify_attribute(GLuint index, GLint size, GLenum type,
                            GLboolean normalized, GLsizei stride,
                            const GLvoid *pointer) {
  bind();
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  if (glGetError() != GL_NO_ERROR) {
    std::cerr << "Error specifying vertex attribute" << std::endl;
  }
  unbind();
}

GLuint VAO::get_id() const { return id; }