#version 420

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec4 vertex_color;
out vec4 aColor;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

void main() {
    gl_Position =  PVM * a_vertex_position;
    aColor = vertex_color;
}
