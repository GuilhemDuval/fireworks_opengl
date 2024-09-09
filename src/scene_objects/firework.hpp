#pragma once
#include "../render/program.hpp"
#include "../render/vao.hpp"
#include "../render/vbo.hpp"
#include "particle.hpp"
#include <glm/gtc/random.hpp>
#include <memory>
#include <vector>

class Firework {
public:
  Firework();

  // Empêcher la copie
  Firework(const Firework &) = delete;
  Firework &operator=(const Firework &) = delete;

  // Permettre le déplacement
  Firework(Firework &&) noexcept = default;
  Firework &operator=(Firework &&) noexcept = default;

  ~Firework() = default;

  bool done() const;
  void run(const glm::vec3 &gravity, const Program &program,
           const glm::mat4 &view_matrix, const glm::mat4 &proj_matrix);

private:
  std::vector<Particle> particles;
  Particle firework;
  float hue;
  std::unique_ptr<VAO> m_vao;
  std::unique_ptr<VBO> m_vbo;

  void draw_particle(const Particle &particle, const Program &program,
                     const glm::mat4 &view_matrix,
                     const glm::mat4 &proj_matrix);
};
