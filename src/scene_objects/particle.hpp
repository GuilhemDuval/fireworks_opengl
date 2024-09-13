#pragma once
#include "../render/program.hpp"
#include "glm/glm.hpp"

class Particle {
public:
  glm::vec3 location;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::vec3 m_color;
  float lifespan; // Durée de vie de la particule
  bool
      seed; // Indique si la particule est une "graine" (feu d'artifice initial)

  Particle(float x, float y, float z, glm::vec3 color);
  Particle(glm::vec3 loc, glm::vec3 color);

  void apply_force(const glm::vec3 &force);
  void update();
  bool is_dead() const;
  bool explode();
};
