#pragma once
#include "../render/program.hpp"
#include "glm/glm.hpp"

class Particle {
public:
  glm::vec3 location;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  float hu;       // Valeur de teinte pour la couleur
  float lifespan; // Durée de vie de la particule
  bool
      seed; // Indique si la particule est une "graine" (feu d'artifice initial)

  Particle(float x, float y, float z, float hue);
  Particle(glm::vec3 loc, float hue);

  void apply_force(const glm::vec3 &force);
  void update();
  bool is_dead() const;
  bool explode();
};
