#include "game_object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "texture_manager.hpp"

GameObject::GameObject(const std::string& model_path, const std::string& texture_path)
    : m_use_texture(true), m_position(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f)), m_scale(1.0f), m_3D_model(Model(model_path))
{
    load_texture(texture_path);
    update_model_matrix();
    set_lighting_factors({1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, 64.0f);
}

GameObject::GameObject(const std::string& model_path, const glm::vec3& color)
    : m_use_texture(false), m_base_color(color), m_position(glm::vec3(0.0f)), m_rotation(glm::vec3(0.0f)), m_scale(1.0f), m_3D_model(model_path)
{
    update_model_matrix();
    set_lighting_factors({1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, 64.0f);
}

void GameObject::set_position(const glm::vec3& new_position)
{
    m_position = new_position;
    update_model_matrix();
}

void GameObject::set_rotation(const glm::vec3& new_rotation)
{
    m_rotation = new_rotation;
    update_model_matrix();
}

void GameObject::set_scale(const glm::vec3& new_scale)
{
    m_scale = new_scale;
    update_model_matrix();
}

void GameObject::set_lighting_factors(const glm::vec3& new_diffuse, const glm::vec3& new_specular, float new_shininess)
{
    m_diffuse_factor   = new_diffuse;
    m_specular_factor  = new_specular;
    m_shininess_factor = new_shininess;
}

void GameObject::draw() const
{
    m_3D_model.draw();
}

void GameObject::load_texture(const std::string& texture_path)
{
    // Load texture from file path
    std::cout << "Loading texture from: " << texture_path << std::endl;

    m_texture_object = TextureManager::load_texture(texture_path);
}

void GameObject::change_texture(const std::string& texture_path)
{
    load_texture(texture_path);
    this->m_use_texture = true;
}

void GameObject::change_color(const glm::vec3& color)
{
    m_base_color = color;
}

void GameObject::update_model_matrix()
{
    m_model_matrix = glm::translate(glm::mat4(1.0f), m_position);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
    m_model_matrix = glm::scale(m_model_matrix, m_scale);
}

void GameObject::move_x(const float offset)
{
    m_position.x += offset;
    update_model_matrix();
}

void GameObject::move_y(const float offset)
{
    m_position.y += offset;
    update_model_matrix();
}

void GameObject::move_z(const float offset)
{
    m_position.z += offset;
    update_model_matrix();
}

void GameObject::move(const glm::vec3 offset)
{
    m_position.x += offset.x;
    m_position.y += offset.y;
    m_position.z += offset.z;
    update_model_matrix();
}

void GameObject::interpolate_material_factors(const glm::vec3& target_diffuse, const glm::vec3& target_specular, float target_shininess, float blend_factor)
{
    m_diffuse_factor   = glm::mix(m_diffuse_factor, target_diffuse, blend_factor);
    m_specular_factor  = glm::mix(m_specular_factor, target_specular, blend_factor);
    m_shininess_factor = glm::mix(m_shininess_factor, target_shininess, blend_factor);
}

void GameObject::setup_matrices(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const glm::mat4& model_matrix)
{
    glm::mat4 MV_matrix     = view_matrix * model_matrix;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(MV_matrix)));
    glm::mat4 MVP_matrix    = proj_matrix * MV_matrix;

    glUniformMatrix4fv(program.u_MVP_matrix, 1, GL_FALSE, glm::value_ptr(MVP_matrix));
    glUniformMatrix4fv(program.u_MV_matrix, 1, GL_FALSE, glm::value_ptr(MV_matrix));
    glUniformMatrix3fv(program.u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
}

void GameObject::setup_shader(Program& program, const glm::vec3& kd, const glm::vec3& ks, float shininess, const glm::vec3& color, bool use_texture)
{
    glUniform3fv(program.u_kd, 1, glm::value_ptr(kd));
    glUniform3fv(program.u_ks, 1, glm::value_ptr(ks));
    glUniform1f(program.u_shininess, shininess);

    if (use_texture && this->get_use_texture())
    {
        glUniform1i(program.u_texture, 0); // Bind texture unit 0
        glBindTexture(GL_TEXTURE_2D, this->get_texture());
        glUniform1i(program.u_use_color, 0); // Signal to use texture
    }
    else
    {
        glUniform1i(program.u_use_color, 1);                     // Signal to use color
        glUniform3fv(program.u_color, 1, glm::value_ptr(color)); // Send the color to the shader
    }
}

void GameObject::render_game_object(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix)
{
    // glCullFace(GL_BACK);

    program.use();

    glm::mat4 model_matrix = this->get_model_matrix();
    setup_matrices(program, view_matrix, proj_matrix, model_matrix);

    glm::vec3 diffuse   = this->get_diffuse_factor();
    glm::vec3 specular  = this->get_specular_factor();
    float     shininess = this->get_shininess_factor();
    setup_shader(program, diffuse, specular, shininess, this->get_base_color(), true);

    this->draw();
}

void GameObject::render_edge(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const float scale_factor)
{
    // glCullFace(GL_FRONT);

    program.use();

    glm::mat4 model_matrix = glm::scale(this->get_model_matrix(), {scale_factor, scale_factor, scale_factor});
    setup_matrices(program, view_matrix, proj_matrix, model_matrix);

    glm::vec3 white(1.0f, 1.0f, 1.0f);
    glm::vec3 black(0.0f, 0.0f, 0.0f);
    setup_shader(program, white, white, 0.0f, black, false);

    this->draw();
}