#include "vbo.hpp"

VBO::VBO()
{
    glGenBuffers(1, &id);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::fill(const void* data, GLsizei size, GLenum usage)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}