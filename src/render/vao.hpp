#pragma once

#include <cstddef>
#include "p6/p6.h"

class VAO {
public:
    VAO();
    ~VAO();

    void bind() const;
    void unbind() const;
    void specify_attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

private:
    GLuint id;
};