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
uniform bool u_is_seed;

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

// Function to compute Blinn-Phong lighting
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
        // Calculate distance between particle and camera
        float distance_from_camera = length(v_position_vs);

        // Make particles smaller for a more realistic firework effect
        float adjusted_radius = mix(0.15, 0.05, clamp(1.0 / distance_from_camera, 0.0, 1.0));

        // Compute the distance from the center of the point sprite
        float distance = length(gl_PointCoord - vec2(0.5));

        // Adjust halo effect based on seed status and distance
        float halo_width = u_is_seed ? 0.3 : 0.2;  // Smaller halo for more diffuse particles
        float alpha = smoothstep(adjusted_radius * 1.5, adjusted_radius - halo_width, distance);

        // Create a glow effect with a softer gradient
        float halo = smoothstep(adjusted_radius * 1.2, adjusted_radius - halo_width, distance) -
            smoothstep(adjusted_radius - halo_width, adjusted_radius - 2.0 * halo_width, distance);

        // Reduce the opacity at the center even more for a subtle, transparent core
        vec3 particle_color = u_color * (1.0 - distance * 0.9) + vec3(1.0, 1.0, 1.0) * halo * 0.3;

        // Final color with much lower opacity at the center for softer edges
        f_frag_color = vec4(particle_color, alpha * (1.0 - distance * 0.8));  // More transparent center

        // Add a subtle, randomized glow for seed particles
        if(u_is_seed) {
            float glow = (1.0 - distance / adjusted_radius) * 0.1;  // Reduce glow intensity
            f_frag_color.rgb += vec3(1.0, 0.8, 0.6) * glow;
        }

        // Apply a slight random variation to each particle for a more natural effect
        f_frag_color.rgb *= 1.0 + (fract(sin(dot(v_position_vs.xy, vec2(12.9898, 78.233))) * 43758.5453) * 0.1);
    } else {
        // Standard Blinn-Phong shading for non-particles
        vec3 normal = normalize(v_normal_vs);
        vec4 frag_color;

        // Choose between texture or uniform color
        if(u_use_color) {
            frag_color = vec4(u_color, 1.0);  // Use the uniform color if specified
        } else {
            frag_color = texture(u_texture, v_tex_coords);  // Use the color and alpha from the texture
        }

        // Calculate lighting with Blinn-Phong for both lights
        vec3 lighting = blinn_phong_lighting(0, normal, v_position_vs) + blinn_phong_lighting(1, normal, v_position_vs);
        lighting = clamp(lighting, vec3(0.1), vec3(1.0)); // Clamp lighting to avoid overexposure

        // Combine the fragment color with the calculated lighting, preserving the alpha
        f_frag_color = vec4(frag_color.rgb * lighting, frag_color.a);
        // f_frag_color = vec4(frag_color.rgb, frag_color.a);
    }
}
