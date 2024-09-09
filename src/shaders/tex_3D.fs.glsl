#version 330 core

struct Light {
    vec3 position; // Light position in view space
    vec3 intensity; // Light intensity
};

// Uniforms
uniform sampler2D u_texture;        // Texture sampler
uniform vec3 u_color;               // Uniform color for particles or solid objects
uniform bool u_use_color;           // Flag to toggle between color and texture
uniform bool u_is_particle;         // Flag to toggle between 3D model and particle

uniform vec3 u_kd;                  // Diffuse reflectivity
uniform vec3 u_ks;                  // Specular reflectivity
uniform float u_shininess;          // Shininess for specular highlight

uniform Light u_lights[2];          // Array of lights

// Inputs from the vertex shader
in vec3 v_normal_vs;                // Transformed vertex normal in view space
in vec2 v_tex_coords;               // Texture coordinates from the vertex shader
in vec3 v_position_vs;              // Transformed vertex position in view space

// Output to the framebuffer
out vec4 f_frag_color;

vec3 blinn_phong_lighting(int light_index, vec3 normal, vec3 frag_pos) {
    vec3 light_dir = normalize(u_lights[light_index].position - frag_pos); // Direction from fragment to light
    vec3 view_dir = normalize(-frag_pos); // Direction from fragment to camera
    vec3 half_vector = normalize(light_dir + view_dir);

    float diffuse_factor = max(dot(normal, light_dir), 0.0);
    float specular_factor = pow(max(dot(normal, half_vector), 0.0), u_shininess);

    float distance = length(u_lights[light_index].position - frag_pos);

    // Attenuation factors
    float constant = 1.0;
    float linear = 0.1;
    float quadratic = 0.01;

    float attenuation = 1.0 / ((constant + linear * distance) + (quadratic * (distance * distance)));

    vec3 light_intensity = u_lights[light_index].intensity * attenuation;
    vec3 diffuse_color = light_intensity * u_kd * diffuse_factor;
    vec3 specular_color = light_intensity * u_ks * specular_factor;

    return diffuse_color + specular_color;
}

void main() {
    if(u_is_particle) {
        // Compute the distance from the center of the point sprite
        float distance = length(gl_PointCoord - vec2(0.5));
        float radius = 5; // Adjust the radius as needed
        float halo = smoothstep(radius, radius - 0.1, distance); // Halo effect

        // Final color with halo effect
        f_frag_color = vec4(u_color, 1.0) * halo;
    } else {
        // Standard Blinn-Phong shading for non-particles
        vec3 normal = normalize(v_normal_vs);
        vec3 frag_color;

        // Choose between texture or uniform color
        if(u_use_color) {
            frag_color = u_color;  // Use the uniform color if specified
        } else {
            vec4 texture_color = texture(u_texture, v_tex_coords);
            frag_color = texture_color.rgb;  // Use the color from the texture
        }

        // Calculate lighting with Blinn-Phong for both lights
        vec3 lighting = blinn_phong_lighting(0, normal, v_position_vs) + blinn_phong_lighting(1, normal, v_position_vs);
        lighting = clamp(lighting, vec3(0.1), vec3(1.0)); // Clamp lighting to avoid overexposure

        // Combine the fragment color with the calculated lighting
        f_frag_color = vec4(frag_color * lighting, 1.0);
    }
}
