#version 330 core

struct Light {
    vec3 position;
    vec3 intensity;
};

uniform sampler2D u_texture;
uniform vec3 u_color;
uniform bool u_use_color;

uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shininess;

uniform Light u_lights[2];

in vec3 v_normal_vs;
in vec2 v_tex_coords;
in vec3 v_position_vs;

out vec4 f_frag_color;

vec3 toon_shading(int light_index, vec3 normal, vec3 frag_pos) {
    vec3 light_dir = normalize(u_lights[light_index].position - frag_pos);
    vec3 view_dir = normalize(-frag_pos);

    float diffuse_factor = dot(normal, light_dir);
    diffuse_factor = (diffuse_factor > 0.5) ? 1.0 : (diffuse_factor > 0.2) ? 0.5 : 0.0;  // Threshold the diffuse lighting

    vec3 light_intensity = u_lights[light_index].intensity / 2;

    vec3 diffuse_color = light_intensity * u_kd * diffuse_factor;
    vec3 specular_color = vec3(0.0);  // No specular highlight in cell shading

    return diffuse_color + specular_color;
}

void main() {
    vec3 normal = normalize(v_normal_vs);
    vec3 frag_color;

    if(u_shininess == 0) {
        f_frag_color = vec4(u_color, 1.0);
    } else {
        if(u_use_color) {
            frag_color = u_color;
        } else {
            vec4 texture_color = texture(u_texture, v_tex_coords);
            frag_color = texture_color.rgb;
        }

        vec3 lighting = toon_shading(0, normal, v_position_vs) + toon_shading(1, normal, v_position_vs);
        lighting = max(lighting, vec3(0.1));
        f_frag_color = vec4(frag_color * lighting, 1.0);
    }
}
