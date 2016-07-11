#version 330 core

out vec4 color;

uniform int highlighted;

void main() {
    if (highlighted == 1)
        color = vec4(0.67f, 0.67f, 0.67f, 1.0f);
    else
        color = vec4(0.33f, 0.33f, 0.33f, 1.0f);
}