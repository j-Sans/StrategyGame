#version 330 core

//Preprocessor directives

//Terrain
#define OPEN_TERRAIN 0
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

//Creature
#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image

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