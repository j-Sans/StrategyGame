#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D tex;

void main() {
    color = texture(tex, TexCoords);
}
