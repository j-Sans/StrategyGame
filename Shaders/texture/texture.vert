#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec2 TexCoords;
out vec4 Color;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    TexCoords = vec2((position.x + 1.0f) / 2.0, 1.0 - ((position.y + 1.0f) / 2.0)); //Reversing y coordinates because the image is loaded upside down
    Color = vec4(color, 1.0);
}
