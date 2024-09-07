#pragma once

#include <glm/glm.hpp>
#include <string>
#include "3D_model.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "program.hpp"

class GameObject {
private:
    Model     m_3D_model;
    GLuint    m_texture_object;
    bool      m_use_texture; // Indicates whether to use a texture
    glm::vec3 m_base_color;  // Base color if no texture is used

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_model_matrix; // Transformation matrix of the object

    glm::vec3 m_diffuse_factor;   // Diffuse reflectivity
    glm::vec3 m_specular_factor;  // Specular reflectivity
    float     m_shininess_factor; // Shininess for specular highlight

    void setup_matrices(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const glm::mat4& model_matrix);
    void setup_shader(Program& program, const glm::vec3& kd, const glm::vec3& ks, float shininess, const glm::vec3& color, bool use_texture);

public:
    GameObject(const std::string& model_path, const std::string& texture_path);
    GameObject(const std::string& model_path, const glm::vec3& color);

    void set_position(const glm::vec3& new_position);
    void set_rotation(const glm::vec3& new_rotation);
    void set_scale(const glm::vec3& new_scale);

    void move(const glm::vec3 offset);
    void move_x(const float offset);
    void move_y(const float offset);
    void move_z(const float offset);

    void set_lighting_factors(const glm::vec3& new_diffuse, const glm::vec3& new_specular, float new_shininess);

    glm::vec3 get_base_color() const { return m_base_color; }
    GLuint    get_texture() const { return m_texture_object; }
    bool      get_use_texture() const { return m_use_texture; }

    glm::vec3 get_position() const { return m_position; }
    glm::vec3 get_rotation() const { return m_rotation; }
    glm::vec3 get_scale() const { return m_scale; }
    glm::mat4 get_model_matrix() const { return m_model_matrix; }

    glm::vec3 get_diffuse_factor() const { return m_diffuse_factor; }
    glm::vec3 get_specular_factor() const { return m_specular_factor; }
    float     get_shininess_factor() const { return m_shininess_factor; }

    void load_texture(const std::string& texture_path);
    void update_model_matrix();
    void change_texture(const std::string& texture_path);
    void change_color(const glm::vec3& color);

    void interpolate_material_factors(const glm::vec3& target_diffuse, const glm::vec3& target_specular, float target_shininess, float blend_factor);

    void render_game_object(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix);
    void render_edge(Program& program, const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const float scale_factor);

    void draw() const;
};
