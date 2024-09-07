#pragma once

#include <glm/glm.hpp>

using Color = glm::vec3;

Color hsv_to_rgb(float h, float s, float v);
Color generate_vivid_color();