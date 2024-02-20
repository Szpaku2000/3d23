#version 420

layout(location=0) out vec4 vFragColor;
layout(location=1) in vec4 input_color;
layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color;
};

void main() {
    vFragColor = vec4 (input_color.xyz * color * strength, 1.0);
}

