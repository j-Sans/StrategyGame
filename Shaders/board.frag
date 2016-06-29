#version 330 core

//Preprocessor directives

#define TERRAIN 0.0
#define CREATURE 1.0

//Terrain
#define OPEN_TERRAIN 0.0
#define MOUNTAIN_TERRAIN 1.0
#define WATER_TERRAIN 2
#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

//Creature
#define NO_CREATURE 0.0
#define STICK_FIGURE_CREATURE 1.0 //Simple test creature type using a stick-figure image

//#define TERRAIN 0
//#define CREATURE 1
//
////Terrain
//#define OPEN_TERRAIN 0
//#define MOUNTAIN_TERRAIN 1
//#define WATER_TERRAIN 2
//#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.
//
////Creature
//#define NO_CREATURE 0
//#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image

in vec2 TexCoords;
in vec2 TexType;

//in ivec2 TexType; //First number represents if it is a texture or terrain, and second number represents the respective type

out vec4 color;

uniform sampler2D grassTex;
uniform sampler2D mountainTex;
uniform sampler2D stickFigureTex;

void main() {
    if (TexType.x == TERRAIN) {
        //Draw the ground
        if (TexType.y == MOUNTAIN_TERRAIN) {
            color = texture(mountainTex, TexCoords);
        } else if (TexType.y == OPEN_TERRAIN) {
            color = texture(grassTex, TexCoords);
        }
    } else if (TexType.x == CREATURE) {
        //Draw the creature
        if (TexType.y == STICK_FIGURE_CREATURE) {
            color = texture(stickFigureTex, TexCoords);
        }
    }
}