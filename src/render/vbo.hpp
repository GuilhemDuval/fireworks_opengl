#pragma once

#include "p6/p6.h"

class VBO {
public:
    VBO();
    ~VBO();

    void bind() const;
    void unbind() const;
    void fill(const void* data, GLsizei size, GLenum usage);

private:
    GLuint id;
};