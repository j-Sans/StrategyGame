#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out VS_OUT {
    vec3 color;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position.x, position.y, 0.0f, 1.0f);
    vs_out.color = color;
}