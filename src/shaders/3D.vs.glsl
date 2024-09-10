#version 330 core

// Vertex attributes
layout(location = 0) in vec3 a_vertex_position;      // Vertex position
layout(location = 1) in vec3 a_vertex_normal;        // Vertex normal
layout(location = 2) in vec2 a_vertex_tex_coords;    // Vertex texture coordinates

// Transformation matrices passed as uniforms
uniform mat4 u_MVP_matrix;       // Model-View-Projection matrix
uniform mat4 u_MV_matrix;        // Model-View matrix
uniform mat3 u_normal_matrix;    // Normal matrix for transforming normals
uniform bool u_is_particle;      // Indicates whether the current object is a particle

// Outputs to the fragment shader
out vec3 v_position_vs;          // Transformed vertex position in view space
out vec3 v_normal_vs;            // Transformed vertex normal in view space
out vec2 v_tex_coords;           // Texture coordinates

void main() {
    // Convert position to homogeneous coordinates
    vec4 vertex_position_hom = vec4(a_vertex_position, 1.0);

    if(u_is_particle) {
        // If it's a particle, we skip normals and textures
        v_position_vs = vec3(u_MV_matrix * vertex_position_hom); // No transformation needed for particles
    } else {
        // Transform position and normal for non-particle objects
        v_position_vs = vec3(u_MV_matrix * vertex_position_hom); // Transform position to view space
        v_normal_vs = normalize(u_normal_matrix * a_vertex_normal); // Transform and normalize normal
        v_tex_coords = a_vertex_tex_coords; // Pass texture coordinates
    }

    // Final projected position
    gl_Position = u_MVP_matrix * vertex_position_hom;
    gl_PointSize = 5.0; // Set the size of the particle
}
