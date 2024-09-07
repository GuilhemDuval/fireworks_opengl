#include "3D_model.hpp"
#include "3D_loader/model_loader.hpp"


Model::Model(const std::string& model_path)
{
    // Load model from file path
    std::cout << "Loading model from: " << model_path << std::endl;

    ModelLoader::Model model = ModelLoader::load_model(model_path);

    m_vbo_vertices.bind();
    m_vbo_vertices.fill(model.combined_data.data(), model.combined_data.size() * sizeof(float), GL_STATIC_DRAW);

    m_vao.bind();
    int stride = 8 * sizeof(float);

    m_vao.specify_attribute(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                   // Position attribute
    m_vao.specify_attribute(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // Normal attribute
    m_vao.specify_attribute(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // Texture coordinate attribute
    m_vao.unbind();

    m_vbo_vertices.unbind();

    // Store the data size for use in glDrawArrays
    m_data_size = model.combined_data.size() / 8;
}

void Model::draw() const {
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, m_data_size);
    m_vao.unbind();
}
