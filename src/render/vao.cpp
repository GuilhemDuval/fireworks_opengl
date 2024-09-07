#include "vao.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &id);
}

void VAO::bind() const
{
    glBindVertexArray(id);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}

void VAO::specify_attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}