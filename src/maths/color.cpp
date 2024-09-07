#include "color.hpp"
#include "random_generator.hpp"

Color hsv_to_rgb(float h, float s, float v)
{
    glm::vec3 rgb;
    int       i = static_cast<int>(h * 6);
    float     f = h * 6 - static_cast<float>(i);
    float     p = v * (1 - s);
    float     q = v * (1 - f * s);
    float     t = v * (1 - (1 - f) * s);

    switch (i % 6)
    {
    case 0: rgb.r = v, rgb.g = t, rgb.b = p; break;
    case 1: rgb.r = q, rgb.g = v, rgb.b = p; break;
    case 2: rgb.r = p, rgb.g = v, rgb.b = t; break;
    case 3: rgb.r = p, rgb.g = q, rgb.b = v; break;
    case 4: rgb.r = t, rgb.g = p, rgb.b = v; break;
    default: rgb.r = v, rgb.g = p, rgb.b = q; break;
    }

    return rgb;
}

Color generate_vivid_color()
{
    float h = static_cast<float>(discrete_uniform_distribution(0, 360)) / 360.0f; // Hue value between 0 and 1
    float s = 0.7f;                                                               // High saturation for vivid colors
    float v = 0.9f;                                                               // High value for vivid colors

    glm::vec3 rgb = hsv_to_rgb(h, s, v);

    return (rgb);
}