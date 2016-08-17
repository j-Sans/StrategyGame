#version 330 core

out vec4 color;

in vec3 Color;

void main() {
    color = vec4(Color.x, Color.y, Color.z, 1.0f);
}