#version 420

layout(location=0) out vec4 vFragColor;
in vec4 aColor;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color;
};

void main() {
    vFragColor[0] = aColor[0] * strength * color[0];
    vFragColor[1] = aColor[1] * strength * color[1];
    vFragColor[2] = aColor[2] * strength * color[2];
    vFragColor[3] = aColor[3];
}

