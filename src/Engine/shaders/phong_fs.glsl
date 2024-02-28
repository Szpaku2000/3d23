#version 460

layout(location=0) out vec4 vFragColor;
in vec2 vertex_texcoords;
in vec3 vertex_normals_in_vs;
in vec3 vertex_coords_in_vs;
in vec4 vertex_position;
in vec3 vertex_normals;
uniform sampler2D map_Kd;
uniform vec3 ambient_light;

#if __VERSION__ > 410
layout(std140, binding=0) uniform Modifiers {
#else
    layout(std140) uniform Modifiers {
    #endif
    vec4  Kd;
    vec3  Ks;
    vec4  Ka;
    bool  use_map_Kd;
    float Ns;
} modifiers;

layout(std140, binding=0) uniform Color {
    vec4  Kd;
    bool use_map_Kd;
};

const int MAX_POINT_LIGHTS=24;

struct PointLight {
    vec3 position_in_ws;
    vec3 position_in_vs;
    vec3 color;
    vec3 intensity;
} ;

layout(std140, binding=3) uniform Lights {
    PointLight light[MAX_POINT_LIGHTS];
    int num_lights;
} lights;

void main() {
    vec3 normal = normalize(vertex_normals);
    vec3 diffuse_light, specular_light;

    float Ns = modifiers.Ns;
    vec3 Ks = modifiers.Ks;
    vec4 Kd = modifiers.Kd;

    if (modifiers.use_map_Kd){
        Kd *= texture(map_Kd, vertex_texcoords);
    }

    for(int i = 0; i < lights.num_lights; i++)
    {
        PointLight light = lights.light[i];

        vec3 light_dir = normalize(light.position_in_ws - vertex_position.xyz);
        float diffuse = max(dot(normal, light_dir), 0.0f);
        diffuse_light += diffuse * light.color;

        vec3 view_dir = -normalize(vertex_coords_in_vs);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float specular = pow(max(dot(view_dir, reflect_dir), 0.0f), Ns);
        specular_light += specular * Ks;
    }

    vFragColor.a = Kd.a;
    vFragColor.rgb = Kd.rgb * (diffuse_light + ambient_light + specular_light);
}