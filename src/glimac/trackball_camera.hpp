#ifndef TRACKBALL_CAMERA_HPP
#define TRACKBALL_CAMERA_HPP

#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

class TrackballCamera {
private:
    float     m_distance;
    float     m_angle_x;
    float     m_angle_y;
    glm::vec3 m_center;

public:
    TrackballCamera()
        : m_distance(5.0f), m_angle_x(0.0f), m_angle_y(0.0f) {}

    void move_front(float delta)
    {
        m_distance += delta;
    }

    void rotate_left(float degrees)
    {
        m_angle_y += glm::radians(degrees);
    }

    void rotate_up(float degrees)
    {
        m_angle_x -= glm::radians(degrees);
    }

    void set_center(const glm::vec3& center)
    {
        m_center = center;
    }

    void reset_camera()
    {
        m_angle_x = 0;
        m_angle_y = 0;
    }

    glm::mat4 get_view_matrix() const
    {
        glm::mat4 view_matrix(1.0f);
        view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -m_distance));
        view_matrix = glm::rotate(view_matrix, m_angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
        view_matrix = glm::rotate(view_matrix, m_angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
        view_matrix = glm::translate(view_matrix, -m_center);
        return view_matrix;
    }
};

#endif // TRACKBALL_CAMERA_HPP
