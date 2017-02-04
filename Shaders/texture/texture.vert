#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 color;

out vec2 TexCoords;
out vec4 Color;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    TexCoords = texCoords;
    Color = vec4(color, 1.0);
}
