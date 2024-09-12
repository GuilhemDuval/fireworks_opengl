#include "firework.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Firework::Firework()
    : hue(glm::linearRand(0.f, 255.f)),
      firework(glm::linearRand(-100.f, 0.f), -50.f,
               glm::linearRand(-150.f, 150.f), hue),
      m_vao(std::make_unique<VAO>()), m_vbo(std::make_unique<VBO>()) {

  m_vao->bind();
  m_vbo->bind();

  glm::vec3 point(0.0f, 0.0f, 0.0f);
  m_vbo->fill(&point, sizeof(glm::vec3), GL_STATIC_DRAW);

  m_vao->specify_attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                           (void *)0);

  m_vbo->unbind();
  m_vao->unbind();
}

bool Firework::done() const { return firework.is_dead() && particles.empty(); }

void Firework::run(const glm::vec3 &gravity, const Program &program,
                   const glm::mat4 &view_matrix, const glm::mat4 &proj_matrix) {
  program.use(); // Utilisation du programme pour l'exécution
  m_vao->bind(); // Bind du VAO pour toutes les opérations

  if (!firework.is_dead()) {
    firework.apply_force(gravity);
    firework.update();
    if (firework.explode()) {
      // Générer les particules après l'explosion
      for (int i = 0; i < 750; ++i) {
        particles.push_back(Particle(firework.location, hue));
      }
    }
    glUniform1i(program.u_is_seed, 1);
    draw_particle(firework, program, view_matrix, proj_matrix);
    glUniform1i(program.u_is_seed, 0);
  }

  // Mise à jour des particules et suppression des particules mortes
  for (auto it = particles.begin(); it != particles.end();) {
    it->apply_force(gravity);
    it->update();

    if (it->is_dead()) {
      it = particles.erase(it); // Supprimer les particules mortes
    } else {
      draw_particle(*it, program, view_matrix, proj_matrix);
      ++it;
    }
  }

  m_vao->unbind(); // Unbind après avoir terminé
}

void Firework::draw_particle(const Particle &particle, const Program &program,
                             const glm::mat4 &view_matrix,
                             const glm::mat4 &proj_matrix) {
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), particle.location);
  glm::mat4 MV_matrix = view_matrix * model_matrix;
  glm::mat4 MVP_matrix = proj_matrix * MV_matrix;

  // Envoyer les matrices au shader
  glUniformMatrix4fv(program.u_MVP_matrix, 1, GL_FALSE,
                     glm::value_ptr(MVP_matrix));
  glUniformMatrix4fv(program.u_MV_matrix, 1, GL_FALSE,
                     glm::value_ptr(MV_matrix));

  // Calcul et envoi de la couleur au shader
  glm::vec3 color(particle.hu / 255.0f, 1.0f, particle.lifespan / 255.0f);
  glUniform3fv(program.u_color, 1, glm::value_ptr(color));

  glUniform1i(program.u_use_color, 1);
  glUniform1i(program.u_is_particle, 1);

  // Dessiner les particules (point unique)
  glDrawArrays(GL_POINTS, 0, 1);

  glUniform1i(program.u_is_particle, 0);
}
