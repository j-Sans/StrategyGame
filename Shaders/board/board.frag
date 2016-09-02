#version 330 core

//Preprocessor directives

#define TERRAIN 0
#define CREATURE 1
#define DAMAGE 2
#define CIRCLE 3
#define BUILDING 4

//Controller color
#define PLAYER_0_COLOR vec4(0.6f, 0.3f, 0.0f, 1.0f) //Player 0 has an orange highlight
#define PLAYER_1_COLOR vec4(0.0f, 0.3f, 0.9f, 1.0f) //Player 1 has a blue highlight
#define PLAYER_2_COLOR vec4(0.8f, 0.1f, 0.2f, 1.0f) //Player 2 has a red highlight
#define PLAYER_3_COLOR vec4(0.0f, 0.3f, 0.0f, 1.0f) //Player 3 has a green highlight

//Terrain
#define OPEN_TERRAIN 0 //terrainMovementEnergyCosts[OPEN_TERRAIN] 1
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define FOREST_TERRAIN 3
#define HILL_TERRAIN 4
#define SWAMP_TERRAIN 5
#define ROAD_TERRAIN 6
//to add creature movement modifiers in terrain it needs to be set up as a creature stat.
//#define CARROT_TREE_TERRAIN 7 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

//Creature
#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image
#define SCOUT_PLACEHOLDER 2 //Simple test creature type using an easily identifiable box texture
#define ARCHER_PLACEHOLDER 3 //Simple test creature type using an easily identifiable box texture

//Building
#define NO_BUILDING 0
#define TOWER_BUILDING 1 //Simple test building type using a tower image

in vec2 TexCoords;
in vec4 TileColor;
flat in ivec2 TexType; //First number represents if it is a texture or terrain, and second number represents the respective type

out vec4 color;

uniform sampler2D grassTex;
uniform sampler2D mountainTex;
uniform sampler2D forestTex;
uniform sampler2D stickFigureTex;
uniform sampler2D scoutPTex;
uniform sampler2D archerPTex;
uniform sampler2D towerTex;
uniform sampler2D circleTex;

void main() {
    if (TexType.x == TERRAIN) {
        //Draw the ground
        if (TexType.y == OPEN_TERRAIN) {
            color = TileColor * texture(grassTex, TexCoords);
        } else if (TexType.y == MOUNTAIN_TERRAIN) {
            color = TileColor * texture(mountainTex, TexCoords);
        } else if (TexType.y == WATER_TERRAIN) {
            color = TileColor * texture(mountainTex, TexCoords);
        } else if (TexType.y == FOREST_TERRAIN) {
            color = TileColor * texture(forestTex, TexCoords);
        } /*else if (TexType.y == HILL_TERRAIN) {
            color = TileColor * texture(hillTex, TexCoords);
        } else if (TexType.y == SWAMP_TERRAIN) {
            color = TileColor * texture(swampTex, TexCoords);
        } else if (TexType.y == ROAD_TERRAIN) {
            color = TileColor * texture(roadTex, TexCoords);
        }*/
        
    } else if (TexType.x == CREATURE) {
        //Draw the creature
        if (TexType.y == STICK_FIGURE_CREATURE) {
            color = texture(stickFigureTex, TexCoords);
        }
        else if (TexType.y == SCOUT_PLACEHOLDER) {
            color = texture(scoutPTex, TexCoords);
        }
        else if (TexType.y == ARCHER_PLACEHOLDER) {
            color = texture(archerPTex, TexCoords);
        }
        
    } else if (TexType.x == BUILDING) {
        //Draw the building
        if (TexType.y == TOWER_BUILDING) {
            color = texture(towerTex, TexCoords);
        }
    
    } else if (TexType.x == DAMAGE) {
        //Draw the damage box
        vec4 colorVec = texture(circleTex, TexCoords) * TileColor;
        
        color = colorVec;
        
    } else if (TexType.x == CIRCLE) {
        //Draw the circle under the creature
        
        vec4 controllerColor;
        
        if (TexType.y == 0) //Controller is player 0
            controllerColor = PLAYER_0_COLOR;
        else if (TexType.y == 1) //Controller is player 1
                controllerColor = PLAYER_1_COLOR;
        else if (TexType.y == 2) //Controller is player 2
            controllerColor = PLAYER_2_COLOR;
        else if (TexType.y == 3) //Controller is player 3
            controllerColor = PLAYER_3_COLOR;
        
        vec4 colorVec = texture(circleTex, TexCoords) * controllerColor;
        
        color = vec4(colorVec.xyz, 0.33f * colorVec.w);
        
    }
}