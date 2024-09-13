#ifndef TRACKBALL_CAMERA_HPP
#define TRACKBALL_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class TrackballCamera {
private:
  float m_distance;
  float m_angle_x;
  float m_angle_y;
  glm::vec3 m_center;
  glm::vec3 m_up;
  float m_move_speed;
  float m_rotate_speed;

public:
  TrackballCamera()
      : m_distance(100.0f), m_angle_x(0.0f), m_angle_y(0.0f), m_center(0.0f),
        m_up(0.0f, 1.0f, 0.0f), m_move_speed(0.5f), m_rotate_speed(0.005f) {}

  void move_front(float delta) {
    m_distance -= delta;
    if (m_distance < 0.1f)
      m_distance = 0.1f;
  }

  void rotate_left(float delta) { m_angle_y += delta * m_rotate_speed; }

  void rotate_up(float delta) {
    m_angle_x += delta * m_rotate_speed;
    m_angle_x = glm::clamp(m_angle_x, -glm::pi<float>() / 2.0f + 0.1f,
                           glm::pi<float>() / 2.0f - 0.1f);
  }

  void move_center(const glm::vec3 &direction) {
    // Calculer la direction avant/arrière de la caméra en fonction des angles X
    // et Y
    glm::vec3 forward_direction = glm::vec3(
        std::sin(m_angle_y) * std::cos(m_angle_x), // Composante horizontale
        std::sin(m_angle_x),                       // Composante verticale
        std::cos(m_angle_y) * std::cos(m_angle_x)  // Composante avant/arrière
    );

    // Ajouter la direction multipliée par la vitesse pour l'avant/arrière
    m_center += forward_direction * direction.z * m_move_speed;

    // Gérer le déplacement latéral (gauche/droite)
    glm::vec3 right_direction =
        glm::normalize(glm::cross(forward_direction, m_up));
    m_center += right_direction * direction.x * m_move_speed;

    // Gérer le déplacement vertical (haut/bas)
    m_center += m_up * direction.y * m_move_speed;
  }

  void set_center(const glm::vec3 &center) { m_center = center; }

  void reset_camera() {
    m_angle_x = 0.0f;
    m_angle_y = 0.0f;
    m_distance = 100.0f;
    m_center = glm::vec3(-50.0f, 60.0f, -10.0f);
  }

  glm::mat4 get_view_matrix() const {
    glm::vec3 position = m_center + get_position();
    return glm::lookAt(position, m_center, m_up);
  }

  glm::vec3 get_position() const {
    return glm::vec3(m_distance * std::sin(m_angle_y) * std::cos(m_angle_x),
                     m_distance * std::sin(m_angle_x),
                     m_distance * std::cos(m_angle_y) * std::cos(m_angle_x));
  }

  void print_camera_info() const {
    glm::vec3 position = get_position();
    std::cout << "Camera Info:" << std::endl;
    std::cout << "  Position: (" << position.x << ", " << position.y << ", "
              << position.z << ")" << std::endl;
    std::cout << "  Center: (" << m_center.x << ", " << m_center.y << ", "
              << m_center.z << ")" << std::endl;
    std::cout << "  Distance: " << m_distance << std::endl;
    std::cout << "  Angle X: " << glm::degrees(m_angle_x) << " degrees"
              << std::endl;
    std::cout << "  Angle Y: " << glm::degrees(m_angle_y) << " degrees"
              << std::endl;
  }

  void set_move_speed(float speed) { m_move_speed = speed; }
  void set_rotate_speed(float speed) { m_rotate_speed = speed; }
};

#endif // TRACKBALL_CAMERA_HPP