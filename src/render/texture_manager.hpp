#pragma once

#include <string>
#include "p6/p6.h"

class TextureManager {
public:
    TextureManager() = default;

    // Load a texture from a file and return its OpenGL identifier
    static GLuint load_texture(const std::string& file_path);

    // Bind the texture to the specified texture unit
    static void bind_texture(GLuint texture_id, GLuint texture_unit);

    // Unbind the current texture
    static void unbind_texture();
};
