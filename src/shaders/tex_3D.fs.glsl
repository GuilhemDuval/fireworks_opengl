#version 330 core

struct Light {
    vec3 position; // Direction de la lumière directionnelle pour la lumière 0, sinon position de la lumière pour les autres
    vec3 intensity; // Intensité de la lumière
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

uniform Light u_lights[6];          // Array of lights

// Inputs from the vertex shader
in vec3 v_normal_vs;                // Transformed vertex normal in view space
in vec2 v_tex_coords;               // Texture coordinates from the vertex shader
in vec3 v_position_vs;              // Transformed vertex position in view space

// Output to the framebuffer
out vec4 f_frag_color;

// Function to compute Blinn-Phong lighting
vec3 blinn_phong_lighting(vec3 normal, vec3 frag_pos) {
    vec3 lighting = vec3(0.0);

    for(int i = 0; i < 6; ++i) {
        vec3 light_dir;
        if(i == 0) {
            // Lumière directionnelle (lumière 0)
            light_dir = normalize(-u_lights[i].position); // La position de la lumière est une direction
        } else {
            // Lumières ponctuelles (les autres)
            light_dir = normalize(u_lights[i].position - frag_pos); // Direction de la lumière ponctuelle
        }
        vec3 view_dir = normalize(-frag_pos); // Direction de la vue
        vec3 half_vector = normalize(light_dir + view_dir);

        float diffuse_factor = max(dot(normal, light_dir), 0.0);
        float specular_factor = pow(max(dot(normal, half_vector), 0.0), u_shininess);

        float distance = length(u_lights[i].position - frag_pos);
        vec3 light_intensity;

        if(i == 0) {
            // Lumière directionnelle, pas d'atténuation
            light_intensity = u_lights[i].intensity;
        } else {
            // Lumières ponctuelles avec atténuation
            float constant = 1.0;
            float linear = 0.1;
            float quadratic = 0.01;
            float attenuation = 1.0 / ((constant + linear * distance) + (quadratic * (distance * distance)));
            light_intensity = u_lights[i].intensity * attenuation;
        }

        vec3 diffuse_color = light_intensity * u_kd * diffuse_factor;
        vec3 specular_color = light_intensity * u_ks * specular_factor;

        lighting += diffuse_color + specular_color;
    }

    return lighting;
}

void main() {
    if(u_is_particle) {
        // Calculer la distance entre la particule et la caméra
        float distance_from_camera = length(v_position_vs);

        // Réduire la taille des particules pour un effet de feu d'artifice plus réaliste
        float adjusted_radius = mix(0.15, 0.05, clamp(1.0 / distance_from_camera, 0.0, 1.0));

        // Calculer la distance depuis le centre du sprite de point
        float distance = length(gl_PointCoord - vec2(0.5));

        // Ajuster l'effet de halo en fonction de l'état du seed et de la distance
        float halo_width = u_is_seed ? 0.3 : 0.2;  // Halo plus petit pour des particules plus diffuses
        float alpha = smoothstep(adjusted_radius * 1.5, adjusted_radius - halo_width, distance);

        // Créer un effet de lueur avec un dégradé plus doux
        float halo = smoothstep(adjusted_radius * 1.2, adjusted_radius - halo_width, distance) -
            smoothstep(adjusted_radius - halo_width, adjusted_radius - 2.0 * halo_width, distance);

        // Réduire l'opacité au centre encore plus pour un noyau transparent subtil
        vec3 particle_color = u_color * (1.0 - distance * 0.9) + vec3(1.0, 1.0, 1.0) * halo * 0.3;

        // Couleur finale avec une opacité beaucoup plus faible au centre pour des bords plus doux
        f_frag_color = vec4(particle_color, alpha * (1.0 - distance * 0.8));  // Centre plus transparent

        // Ajouter une lueur subtile et aléatoire pour les particules de seed
        if(u_is_seed) {
            float glow = (1.0 - distance / adjusted_radius) * 0.1;  // Réduire l'intensité de la lueur
            f_frag_color.rgb += vec3(1.0, 0.8, 0.6) * glow;
        }

        // Appliquer une légère variation aléatoire à chaque particule pour un effet plus naturel
        f_frag_color.rgb *= 1.0 + (fract(sin(dot(v_position_vs.xy, vec2(12.9898, 78.233))) * 43758.5453) * 0.1);
    } else {
        // Éclairage Blinn-Phong pour les objets non particules
        vec3 normal = normalize(v_normal_vs);
        vec4 frag_color;

        // Choisir entre texture ou couleur uniforme
        if(u_use_color) {
            frag_color = vec4(u_color, 1.0); // Utiliser la couleur uniforme si spécifié
        } else {
            frag_color = texture(u_texture, v_tex_coords); // Utiliser la couleur et l'alpha de la texture
        }

        // Calculer l'éclairage avec Blinn-Phong pour toutes les lumières
        vec3 lighting = blinn_phong_lighting(normal, v_position_vs);
        lighting = clamp(lighting, vec3(0.1), vec3(1.0)); // Limiter l'éclairage pour éviter la surexposition

        if(u_shininess < 0.1) {
            f_frag_color = vec4(frag_color.rgb * 0.75, frag_color.a);
        } else {
            // Combiner la couleur du fragment avec l'éclairage calculé, en préservant l'alpha
            f_frag_color = vec4(frag_color.rgb * lighting, frag_color.a);
        }

    }
}
