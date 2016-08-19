#version 330 core

out vec4 color;

in float Color;

void main() {
    color = vec4(Color, Color, Color, 1.0f);
}