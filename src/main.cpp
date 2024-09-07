#include "glimac/trackball_camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "render/game_object.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "maths/color.hpp"
#include "maths/random_generator.hpp"
#include "render/program.hpp"

struct Light {
  glm::vec3 position;  // Light position in view space
  glm::vec3 intensity; // Light intensity
};

int time_events(int next_event_time, p6::Context &ctx) {
  const double current_time = ctx.time();
  const double lambda = 1.0 / 3; // Every 3 seconds

  if (current_time >= next_event_time) {
    double delay = exponential_distribution(lambda);
    next_event_time = current_time + delay;
    std::cout << "Next event in " << delay << " seconds." << std::endl;
  }
  return next_event_time;
}

void handle_camera_input(p6::Context &ctx, TrackballCamera &camera,
                         float &last_x, float &last_y) {
  ctx.mouse_dragged = [&](p6::MouseDrag drag) {
    float deltaX = drag.position.x - last_x;
    float deltaY = drag.position.y - last_y;

    if (last_x != 0 && last_y != 0) {
      camera.rotate_left(-deltaX * 25.f);
      camera.rotate_up(deltaY * 25.f);
    }

    last_x = drag.position.x;
    last_y = drag.position.y;
  };

  if (!ctx.mouse_button_is_pressed(p6::Button::Left)) {
    last_x = 0;
    last_y = 0;
  }

  if (ctx.mouse_button_is_pressed(p6::Button::Right)) {
    camera.reset_camera();
  }

  ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
    camera.move_front(-scroll.dy);
  };
}

int main() {
  auto ctx = p6::Context{{1280, 720, "Projet d'honneur - Guilhem Duval"}};
  ctx.maximize_window();
  glEnable(GL_DEPTH_TEST);

  // Seed the random number generator
  srand(time(NULL));

  TrackballCamera camera;
  Program program{};
  Light lights[2];

  double next_event_time = 0.0;

  GameObject space_object("assets/models/space.obj",
                          "assets/textures/space_texture.jpg");
  space_object.set_scale(glm::vec3(0.1f, 0.1f, 0.1f));
  space_object.set_lighting_factors({0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
                                    75.0f);

  float last_x = 0;
  float last_y = 0;

  glm::vec3 lightPosition(0.0f, 0.0f, 0.0f);
  float lightMotionRadius = 8.0f;
  float lightMotionSpeed = 0.5f;
  lights[0].intensity = glm::vec3(2.0f, 2.0f, 2.0f);

  ctx.update = [&]() {
    next_event_time = time_events(next_event_time, ctx);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.set_center({0., 0., 0.});
    handle_camera_input(ctx, camera, last_x, last_y);

    glm::mat4 view_matrix = camera.get_view_matrix();
    glm::mat4 proj_matrix =
        glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // Update light position
    float time = ctx.time();
    lightPosition.x = sin(time * lightMotionSpeed) * lightMotionRadius;
    lightPosition.z = cos(time * lightMotionSpeed) * lightMotionRadius;

    lights[0].position = glm::vec3(view_matrix * glm::vec4(lightPosition, 1.0));

    lights[1].position = glm::vec3(view_matrix * glm::vec4(0., 0., 0., 1.0));

    lights[1].intensity = glm::vec3(2.0f, 2.0f, 2.0f);

    program.use();
    glUniform3fv(program.u_light_pos_vs_0, 1,
                 glm::value_ptr(lights[0].position));
    glUniform3fv(program.u_light_intensity_0, 1,
                 glm::value_ptr(lights[0].intensity));
    glUniform3fv(program.u_light_pos_vs_1, 1,
                 glm::value_ptr(lights[1].position));
    glUniform3fv(program.u_light_intensity_1, 1,
                 glm::value_ptr(lights[1].intensity));

    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);
    space_object.render_game_object(program, view_matrix, proj_matrix);

    glCullFace(GL_BACK);

    glDisable(GL_CULL_FACE);
  };

  ctx.start();
}
