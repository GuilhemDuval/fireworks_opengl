#include "glimac/trackball_camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
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
#include "scene_objects/firework.hpp"

struct Light {
  glm::vec3 position;  // Light position in view space
  glm::vec3 intensity; // Light intensity
};

// Variables globales
std::vector<Firework> fireworks;
glm::vec3 gravity(0.f, -0.2f, 0.f);

void update_fireworks(Program &program, const glm::mat4 &view_matrix,
                      const glm::mat4 &proj_matrix) {
  if (glm::linearRand(0.f, 10.f) < 0.2f) {
    fireworks.push_back(Firework());
    std::cout << "2" << std::endl;
  }

  for (auto it = fireworks.begin(); it != fireworks.end();) {
    it->run(gravity, program, view_matrix, proj_matrix);
    if (it->done()) {
      it = fireworks.erase(it);
    } else {
      ++it;
    }
  }
}

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

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar *message,
                                     const void *userParam) {
  std::cout << "GL CALLBACK: "
            << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
            << " type = " << type << ", severity = " << severity
            << ", message = " << message << std::endl;
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

  GameObject arrow_z("assets/models/arrow.obj", glm::vec3(0., 0., 1.));
  arrow_z.set_scale(glm::vec3(5.f, 5.f, 5.f));

  GameObject arrow_x("assets/models/arrow.obj", glm::vec3(1., 0., 0.));
  arrow_x.set_scale(glm::vec3(5.f, 5.f, 5.f));
  arrow_x.set_rotation(glm::vec3(90., 0., 0.));

  GameObject arrow_y("assets/models/arrow.obj", glm::vec3(0., 1., 0.));
  arrow_y.set_scale(glm::vec3(5.f, 5.f, 5.f));
  arrow_y.set_rotation(glm::vec3(0., 0., -90.));

  GameObject ef_dushBoard("assets/models/ef_dushBoard.obj",
                          "assets/textures/ef_dushBoard.png");
  GameObject ef_hpipeBoard("assets/models/ef_hpipeBoard.obj",
                           "assets/textures/ef_hpipeBoard.png");
  GameObject ef_hpipeBoard2("assets/models/ef_hpipeBoard2.obj",
                            "assets/textures/ef_hpipeBoard2.png");
  GameObject ef_hpipeBoard3("assets/models/ef_hpipeBoard3.obj",
                            "assets/textures/ef_hpipeBoard3.png");
  GameObject shadow("assets/models/shadow.obj", "assets/textures/shadow.png");
  GameObject TR_caveWall("assets/models/TR_caveWall.obj",
                         "assets/textures/TR_caveWall.png");
  GameObject TR_chiso("assets/models/TR_chiso.obj",
                      "assets/textures/TR_chiso.png");
  GameObject TR_hari("assets/models/TR_hari.obj",
                     "assets/textures/TR_hari.png");
  GameObject TR_hasira("assets/models/TR_hasira.obj",
                       "assets/textures/TR_hasira.png");
  GameObject TR_houseALL("assets/models/TR_houseALL.obj",
                         "assets/textures/TR_houseALL.png");
  GameObject TR_iwa("assets/models/TR_iwa.obj", "assets/textures/TR_iwa.png");
  GameObject TR_iwa2("assets/models/TR_iwa2.obj",
                     "assets/textures/TR_iwa2.png");
  GameObject TR_jimen("assets/models/TR_jimen.obj",
                      "assets/textures/TR_jimen.png");
  GameObject TR_joint("assets/models/TR_joint.obj",
                      "assets/textures/TR_joint.png");
  GameObject TR_kanbanALL("assets/models/TR_kanbanALL.obj",
                          "assets/textures/TR_kanbanALL.png");
  GameObject TR_senro_ura("assets/models/TR_senro_ura.obj",
                          "assets/textures/TR_senro_ura.png");
  GameObject TR_senro("assets/models/TR_senro.obj",
                      "assets/textures/TR_senro.png");
  GameObject TR_SF_shadow("assets/models/TR_SF_shadow.obj",
                          "assets/textures/TR_SF_shadow.png");
  GameObject TR_sky("assets/models/TR_sky.obj", "assets/textures/TR_sky.png");
  TR_sky.set_scale(glm::vec3(.9f, 0.9f, 0.9f));
  GameObject TR_spot1("assets/models/TR_spot1.obj",
                      "assets/textures/TR_spot1.png");
  GameObject TR_teppan("assets/models/TR_teppan.obj",
                       "assets/textures/TR_teppan.png");
  GameObject TR_tesuri("assets/models/TR_tesuri.obj",
                       "assets/textures/TR_tesuri.png");
  GameObject TR_wood("assets/models/TR_wood.obj",
                     "assets/textures/TR_wood.png");

  GameObject space_object("assets/models/space.obj",
                          "assets/textures/space_texture.jpg");
  space_object.set_scale(glm::vec3(1.5f, 1.5f, 1.5f));
  space_object.set_lighting_factors({0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
                                    75.0f);

  GameObject star_boid("assets/models/star_low.obj", generate_vivid_color());
  star_boid.set_scale(glm::vec3(0.01f, 0.01f, 0.001f));
  star_boid.set_lighting_factors({1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, 200.0f);

  float last_x = 0;
  float last_y = 0;

  glm::vec3 lightPosition(0.0f, 0.0f, 0.0f);
  float lightMotionRadius = 8.0f;
  float lightMotionSpeed = 0.5f;
  lights[0].intensity = glm::vec3(20.0f, 20.0f, 20.0f);

  ctx.update = [&]() {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglCallbackFunction, nullptr);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);

    next_event_time = time_events(next_event_time, ctx);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera.set_center({100., 0., 0.});
    handle_camera_input(ctx, camera, last_x, last_y);

    glm::mat4 view_matrix = camera.get_view_matrix();
    glm::mat4 proj_matrix =
        glm::perspective(glm::radians(90.f), ctx.aspect_ratio(), 0.1f, 2000.f);

    // Update light position
    float time = ctx.time();
    lightPosition.x = sin(time * lightMotionSpeed) * lightMotionRadius;
    lightPosition.z = cos(time * lightMotionSpeed) * lightMotionRadius;

    lights[0].position = glm::vec3(view_matrix * glm::vec4(lightPosition, 1.0));

    lights[1].position =
        glm::vec3(view_matrix * glm::vec4(-30., 50., -30., 1.0));

    lights[1].intensity = glm::vec3(0.0f, 20.0f, 20.0f);

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
    std::cout << "1";

    TR_sky.render_game_object(program, view_matrix, proj_matrix);

    ef_dushBoard.render_game_object(program, view_matrix, proj_matrix);

    // ef_hpipeBoard3.render_game_object(program, view_matrix, proj_matrix);
    ef_hpipeBoard.render_game_object(program, view_matrix, proj_matrix);
    ef_hpipeBoard2.render_game_object(program, view_matrix, proj_matrix);

    TR_caveWall.render_game_object(program, view_matrix, proj_matrix);
    TR_chiso.render_game_object(program, view_matrix, proj_matrix);
    TR_hari.render_game_object(program, view_matrix, proj_matrix);
    TR_hasira.render_game_object(program, view_matrix, proj_matrix);
    TR_houseALL.render_game_object(program, view_matrix, proj_matrix);
    TR_iwa.render_game_object(program, view_matrix, proj_matrix);
    TR_iwa2.render_game_object(program, view_matrix, proj_matrix);
    TR_jimen.render_game_object(program, view_matrix, proj_matrix);
    TR_joint.render_game_object(program, view_matrix, proj_matrix);
    TR_kanbanALL.render_game_object(program, view_matrix, proj_matrix);
    TR_senro_ura.render_game_object(program, view_matrix, proj_matrix);
    TR_senro.render_game_object(program, view_matrix, proj_matrix);

    TR_spot1.render_game_object(program, view_matrix, proj_matrix);
    TR_teppan.render_game_object(program, view_matrix, proj_matrix);
    TR_tesuri.render_game_object(program, view_matrix, proj_matrix);
    TR_wood.render_game_object(program, view_matrix, proj_matrix);

    TR_SF_shadow.render_game_object(program, view_matrix, proj_matrix);
    shadow.render_game_object(program, view_matrix, proj_matrix);

    glCullFace(GL_FRONT);
    // space_object.render_game_object(program, view_matrix, proj_matrix);

    glCullFace(GL_BACK);
    arrow_z.render_game_object(program, view_matrix, proj_matrix);
    arrow_y.render_game_object(program, view_matrix, proj_matrix);
    arrow_x.render_game_object(program, view_matrix, proj_matrix);

    glDisable(GL_CULL_FACE);
    update_fireworks(program, view_matrix, proj_matrix);
  };

  ctx.start();
}
