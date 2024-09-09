#include "particle.hpp"
#include "glm/gtc/random.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Particle::Particle(float x, float y, float z, float hue)
    : location(x, y, z), hu(hue), lifespan(255.0), seed(true) {
  velocity = glm::vec3(0, glm::linearRand(-25.f, -10.f), 0);
  acceleration = glm::vec3(0, 0, 0);
}

Particle::Particle(glm::vec3 loc, float hue)
    : location(loc), hu(hue), lifespan(255.0), seed(false) {
  velocity = glm::ballRand(1.0f) * glm::linearRand(4.f, 8.f);
  acceleration = glm::vec3(0, 0, 0);
}

void Particle::apply_force(const glm::vec3 &force) { acceleration += force; }

void Particle::update() {
  velocity += acceleration;
  location += velocity;
  if (!seed) {
    lifespan -= 5.0f;
    velocity *= 0.9f; // Réduction de la vitesse au fil du temps
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
