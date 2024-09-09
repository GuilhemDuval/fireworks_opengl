#pragma once

#include "vao.hpp"
#include "vbo.hpp"
#include <glm/glm.hpp>
#include <string>

class Model {
public:
  explicit Model(const std::string &model_path);

  // Empêcher la copie
  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  // Permettre le déplacement
  Model(Model &&) noexcept = default;
  Model &operator=(Model &&) noexcept = default;

  ~Model() = default;

  const VAO &get_VAO() const { return m_vao; }
  void draw() const;

private:
  int m_data_size;
  VAO m_vao;
  VBO m_vbo_vertices;
};