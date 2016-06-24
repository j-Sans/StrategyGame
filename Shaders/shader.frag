#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D grassTex;

void main() {
    color = texture(grassTex, TexCoords);
}