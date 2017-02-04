#version 330 core

out vec4 color;

in vec2 TexCoords;
in vec4 Color;

uniform sampler2D tex;

void main() {
    color = texture(tex, TexCoords) * Color;
}
