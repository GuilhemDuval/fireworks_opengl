#pragma once

#include <glm/glm.hpp>
#include "vao.hpp"
#include "vbo.hpp"

class Model {
private:
    int m_data_size;
    VAO m_vao;
    VBO m_vbo_vertices;

public:
    Model(const std::string& model_path);
    
    VAO get_VAO() const { return m_vao; }
    void draw() const;
};