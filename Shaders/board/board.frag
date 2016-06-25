#version 330 core

//Preprocessor directives
#define OPEN_TERRAIN 0.0f
#define MOUNTAIN_TERRAIN 1.0f

in vec3 TexCoords;

out vec4 color;

uniform sampler2D grassTex;
uniform sampler2D mountainTex;

void main() {
    if (TexCoords.z == MOUNTAIN_TERRAIN) {
        color = texture(mountainTex, TexCoords.xy);
    } else {
        color = texture(grassTex, TexCoords.xy);
    }
}