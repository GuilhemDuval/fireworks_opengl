#include "particle.hpp"
#include "glm/gtc/random.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Particle::Particle(float x, float y, float z, glm::vec3 color)
    : location(x, y, z), m_color(color), lifespan(255.0), seed(true) {
  velocity = glm::vec3(0, glm::linearRand(4.f, 8.f), 0);
  acceleration = glm::vec3(0, 0, 0);
}

Particle::Particle(glm::vec3 loc, glm::vec3 color)
    : location(loc), m_color(color), lifespan(255.0), seed(false) {
  velocity = glm::ballRand(1.0f) * glm::linearRand(1.f, 3.f);
  acceleration = glm::vec3(0, 0, 0);
}

void Particle::apply_force(const glm::vec3 &force) { acceleration += force; }

void Particle::update() {
  velocity += acceleration;
  location += velocity;
  if (!seed) {
    lifespan -= 2.5f;
    velocity *= 0.90f; // Réduction de la vitesse au fil du temps
  }
  acceleration = glm::vec3(0, 0, 0); // Réinitialise l'accélération
}

bool Particle::is_dead() const { return lifespan <= 0.0f; }

bool Particle::explode() {
  if (seed && velocity.y < 0) {
    lifespan = 0.0f;
    return true;
  }
  return false;
}
