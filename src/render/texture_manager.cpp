#include "texture_manager.hpp"

GLuint TextureManager::load_texture(const std::string& file_path)
{
    // Load image from file
    img::Image texture_image = p6::load_image_buffer(file_path);

    // Generate the OpenGL texture object
    GLuint texture_object = 0;
    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_2D, texture_object);

    // Configure texture settings
    GLsizei width  = static_cast<GLsizei>(texture_image.width());
    GLsizei height = static_cast<GLsizei>(texture_image.height());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Unbind the texture
    unbind_texture();

    return texture_object;
}

void TextureManager::bind_texture(GLuint texture_id, GLuint texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void TextureManager::unbind_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
