#pragma once

#include "p6/p6.h"
#include <cstddef>

class VAO {
public:
  VAO();
  ~VAO();

  // Empêcher la copie
  VAO(const VAO &) = delete;
  VAO &operator=(const VAO &) = delete;

  void bind() const;
  void unbind() const;
  void specify_attribute(GLuint index, GLint size, GLenum type,
                         GLboolean normalized, GLsizei stride,
                         const GLvoid *pointer);
  GLuint get_id() const;

private:
  GLuint id;
};