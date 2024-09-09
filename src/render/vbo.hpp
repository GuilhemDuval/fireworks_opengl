#pragma once

#include "p6/p6.h"

class VBO {
public:
  VBO();
  ~VBO();

  // Empêcher la copie
  VBO(const VBO &) = delete;
  VBO &operator=(const VBO &) = delete;

  void bind() const;
  void unbind() const;
  void fill(const void *data, GLsizei size, GLenum usage);
  void update(const void *data, GLsizei size, GLintptr offset = 0);
  GLuint get_id() const;

private:
  GLuint id;
};