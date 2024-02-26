#version 420

layout(location=0) out vec4 vFragColor;
in vec4 aColor;
in vec3 vertex_normals_in_vs;
in vec3 vertex_coords_in_vs;
in vec2 vertex_texcoords;
uniform sampler2D map_Kd;

layout(std140, binding=0) uniform Color {
    vec4  Kd;
    bool use_map_Kd;
};

void main() {
//    vec3 normal = normalize(vertex_normals_in_vs);
    if (!use_map_Kd){
        vFragColor = Kd;
    } else {
        vFragColor = Kd*texture(map_Kd, vertex_texcoords);
    }
    vFragColor.rgb = abs(vertex_coords_in_vs);
}