#version 410

layout(location=0) out vec4 vFragColor;
in vec4 aColor;

void main() {
    vFragColor = aColor;
}
