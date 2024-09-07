#version 330 core

struct Light {
    vec3 position; // Light position in view space
    vec3 intensity; // Light intensity
};

uniform sampler2D u_texture;        // Texture sampler
uniform vec3 u_color;               // Uniform color for non-textured rendering
uniform bool u_use_color;           // Flag to toggle between color and texture

uniform vec3 u_kd;                  // Diffuse reflectivity
uniform vec3 u_ks;                  // Specular reflectivity
uniform float u_shininess;          // Shininess for specular highlight

uniform Light u_lights[2];

in vec3 v_normal_vs;                // Transformed vertex normal in view space
in vec2 v_tex_coords;               // Texture coordinates from the vertex shader
in vec3 v_position_vs;              // Transformed vertex position in view space (should be passed from vertex shader)

out vec4 f_frag_color;              // Output fragment color

vec3 blinn_phong_lighting(int light_index, vec3 normal, vec3 frag_pos) {
    vec3 light_dir = normalize(u_lights[light_index].position - frag_pos); // Direction from fragment to light
    vec3 view_dir = normalize(-frag_pos); // Direction from fragment to camera
    vec3 half_vector = normalize(light_dir + view_dir);

    float diffuse_factor = max(dot(normal, light_dir), 0.0);
    float specular_factor = pow(max(dot(normal, half_vector), 0.0), u_shininess);

    float distance = length(u_lights[light_index].position - frag_pos);
    // float attenuation = 1.0 / (distance * distance); // Simple quadratic attenuation

    // Very low attenuation coefficients
    float constant = 1.0;    // Constant coefficient, does not change with distance
    float linear = 0.1;   // Very low linear coefficient
    float quadratic = 0.01; // Very low quadratic coefficient

    float attenuation = 1.0 / ((constant + linear * distance) + (quadratic * (distance * distance)));

    vec3 light_intensity = u_lights[light_index].intensity * attenuation;
    vec3 diffuse_color = light_intensity * u_kd * diffuse_factor;
    vec3 specular_color = light_intensity * u_ks * specular_factor;

    return diffuse_color + specular_color;
}

void main() {
    vec3 normal = normalize(v_normal_vs);  // Normalize the normal vector
    vec3 frag_color;

    if(u_shininess == 0) {
        f_frag_color = vec4(u_color, 1.0);
    } else {
        if(u_use_color) {
            frag_color = u_color;  // Use the uniform color if specified
        } else {
            vec4 texture_color = texture(u_texture, v_tex_coords);
            frag_color = texture_color.rgb;  // Use the color from the texture
        }

        vec3 lighting = blinn_phong_lighting(0, normal, v_position_vs) + blinn_phong_lighting(1, normal, v_position_vs); // Calculate lighting
        lighting = clamp(lighting, vec3(0.1), vec3(1.0));
        f_frag_color = vec4(frag_color * lighting, 1.0);  // Apply lighting to the fragment color
    }

}
