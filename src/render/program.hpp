#pragma once
// #include "game_object.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Program {
public:
    p6::Shader m_program;

    GLint u_MVP_matrix;
    GLint u_MV_matrix;
    GLint u_normal_matrix;

    GLint u_texture;
    GLint u_color;
    GLint u_use_color;

    GLint u_kd;
    GLint u_ks;
    GLint u_shininess;

    GLint u_light_pos_vs_0;
    GLint u_light_intensity_0;
    GLint u_light_pos_vs_1;
    GLint u_light_intensity_1;

    Program()
        : m_program{p6::load_shader(
            "../src/shaders/3D.vs.glsl",
            "../src/shaders/tex_3D.fs.glsl"
        )}
        , u_MVP_matrix(glGetUniformLocation(m_program.id(), "u_MVP_matrix"))
        , u_MV_matrix(glGetUniformLocation(m_program.id(), "u_MV_matrix"))
        , u_normal_matrix(glGetUniformLocation(m_program.id(), "u_normal_matrix"))
        , u_texture(glGetUniformLocation(m_program.id(), "u_texture"))
        , u_color(glGetUniformLocation(m_program.id(), "u_color"))
        , u_use_color(glGetUniformLocation(m_program.id(), "u_use_color"))
        , u_kd(glGetUniformLocation(m_program.id(), "u_kd"))
        , u_ks(glGetUniformLocation(m_program.id(), "u_ks"))
        , u_shininess(glGetUniformLocation(m_program.id(), "u_shininess"))
        , u_light_pos_vs_0(glGetUniformLocation(m_program.id(), "u_lights[0].position"))
        , u_light_intensity_0(glGetUniformLocation(m_program.id(), "u_lights[0].intensity"))
        , u_light_pos_vs_1(glGetUniformLocation(m_program.id(), "u_lights[1].position"))
        , u_light_intensity_1(glGetUniformLocation(m_program.id(), "u_lights[1].intensity"))

    {
    }

    void use() const;
};
