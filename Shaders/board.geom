#version 330 core

//Preprocessor directives

#define TERRAIN 0
#define CREATURE 1
#define DAMAGE 2
#define CIRCLE 3
#define BUILDING 4

//Direction
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//Terrain
#define OPEN_TERRAIN 0
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

//Creature
#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image

//Building
#define NO_BUILDING 0
#define TOWER_BUILDING 1 //Simple test building using a tower image

layout (points) in;
layout (triangle_strip, max_vertices = 16) out;

//4 vertices for tile, 4 vertices for creature, 4 vertices for the damage box, and 4 for the circle

//Tile
in int terrain[];
in vec4 tileColor[];

//Creature
in int creature[];
in int creatureDirection[];
in int creatureController[];
in int creatureDamage[];
in float creatureOffset[];

//Building
in int building[];
in int buildingController[];

out vec2 TexCoords;
out vec4 TileColor;
flat out ivec2 TexType; //First number represents if it is a texture or terrain, and second number represents the respective type

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;
uniform mat4 creatureMat;

//uniform float slope;


//Functions
void makeOpen(vec4 position);
void makeMountain(vec4 position);
void drawCreature(vec4 position, int creatureTypeToDraw, vec4 rect[4]);
void drawBuilding(vec4 position, int buildingTypeToDraw, vec4 square[4]);
void drawDamageBox(vec4 position, int damage, vec4 rect[4]);
//void drawCircle(vec4 position);

void main() {
    vec4 position = gl_in[0].gl_Position;
    
    //1x2 Rectangle coordinates with these transformations
    vec4 rect[] = vec4[](
        vec4( 0.45f,  0.35f, 0.0f, 0.0f),
        vec4( 0.05f, -0.05f, 0.0f, 0.0f),
        vec4( 0.35f,  0.45f, 0.0f, 0.0f),
        vec4(-0.05f,  0.05f, 0.0f, 0.0f)
    );
    
    vec4 square[] = vec4[](
       vec4( 0.30f,  0.10f, 0.0f, 0.0f),
       vec4( 0.10f, -0.10f, 0.0f, 0.0f),
       vec4( 0.10f,  0.30f, 0.0f, 0.0f),
       vec4(-0.10f,  0.10f, 0.0f, 0.0f)
    );
    
    //Draw the ground
    if (terrain[0] == OPEN_TERRAIN) {
        makeOpen(position);
    } else if (terrain[0] == MOUNTAIN_TERRAIN) {
        makeMountain(position);
    }
    
    //Draw present building
    drawBuilding(position, building[0], square);
    
    //Draw present creature
    drawCreature(position, creature[0], rect);
    
    //Draw damage box
    if (creatureDamage[0] != 0)
        drawDamageBox(position, creatureDamage[0], rect);
    
}

void makeOpen(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    TexCoords = vec2(0.0f, 0.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    TexCoords = vec2(1.0f, 0.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    TexCoords = vec2(0.0f, 1.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    TexCoords = vec2(1.0f, 1.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void makeMountain(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom
    TexCoords = vec2(0.0f, 1.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Right
    TexCoords = vec2(0.5f, 1.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Left
    TexCoords = vec2(0.0f, 0.5f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.2f,  0.2f, 0.0f, 0.0f)); //Top
    TexCoords = vec2(1.0f, 0.0f);
    TileColor = tileColor[0];
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

//Note: This function appears to use seemingly random complex numbers for coordinates, but they have been calculated to ensure proper proportions for humanoid creatures
//The coordinates that are added to the transformation matrices make a rectange 4 times as tall as wide. Since the matrices compress it by a half, it fits with a 1x2 image for a humanoid
void drawCreature(vec4 position, int creatureTypeToDraw, vec4 rect[4]) {
    if (creatureDirection[0] == NORTH) {
        for (int a = 0; a < 4; a++) {
            rect[a].y += creatureOffset[0];
        }
    } else if (creatureDirection[0] == WEST) {
        for (int a = 0; a < 4; a++) {
            rect[a].x -= creatureOffset[0];
        }
    } else if (creatureDirection[0] == SOUTH) {
        for (int a = 0; a < 4; a++) {
            rect[a].y -= creatureOffset[0];
        }
    } else if (creatureDirection[0] == EAST) {
        for (int a = 0; a < 4; a++) {
            rect[a].x += creatureOffset[0];
        }
    }
    
    if (creatureTypeToDraw != NO_CREATURE) {
        
        //Draw the circle for the creature
        
        //The positions of a diamond in the shape of the tile
        vec4 tileDiamond[] = vec4[](
            vec4(-0.1f, -0.1f, 0.0f, 0.0f),
            vec4( 0.1f, -0.1f, 0.0f, 0.0f),
            vec4(-0.1f,  0.1f, 0.0f, 0.0f),
            vec4( 0.1f,  0.1f, 0.0f, 0.0f)
        );
        
        if (creatureDirection[0] == NORTH) {
            for (int a = 0; a < 4; a++) {
                tileDiamond[a].y += creatureOffset[0];
            }
        } else if (creatureDirection[0] == WEST) {
            for (int a = 0; a < 4; a++) {
                tileDiamond[a].x -= creatureOffset[0];
            }
        } else if (creatureDirection[0] == SOUTH) {
            for (int a = 0; a < 4; a++) {
                tileDiamond[a].y -= creatureOffset[0];
            }
        } else if (creatureDirection[0] == EAST) {
            for (int a = 0; a < 4; a++) {
                tileDiamond[a].x += creatureOffset[0];
            }
        }
        
        //Draw the circle
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[0])); //Bottom
        TexCoords = vec2(0.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, creatureController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[1])); //Right
        TexCoords = vec2(0.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, creatureController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[2])); //Left
        TexCoords = vec2(1.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, creatureController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[3])); //Top
        TexCoords = vec2(1.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, creatureController[0]);
        EmitVertex();
        
        EndPrimitive();
        
        
        //Draw the creature after
        
        gl_Position = ortho * view * model * (position + (0.5f * rect[0])); //Top right
        TexCoords = vec2(0.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * rect[1])); //Bottom right
        TexCoords = vec2(0.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * rect[2])); //Top left
        TexCoords = vec2(1.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * rect[3])); //Bottom left
        TexCoords = vec2(1.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        EndPrimitive();
    }
}

void drawBuilding(vec4 position, int buildingTypeToDraw, vec4 square[4]) {
    if (buildingTypeToDraw != NO_BUILDING) {
        
        //Draw the circle for the creature
        
        //The positions of a diamond in the shape of the tile
        vec4 tileDiamond[] = vec4[](
            vec4(-0.1f, -0.1f, 0.0f, 0.0f),
            vec4( 0.1f, -0.1f, 0.0f, 0.0f),
            vec4(-0.1f,  0.1f, 0.0f, 0.0f),
            vec4( 0.1f,  0.1f, 0.0f, 0.0f)
        );
        
        //Draw the circle
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[0])); //Bottom
        TexCoords = vec2(0.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, buildingController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[1])); //Right
        TexCoords = vec2(0.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, buildingController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[2])); //Left
        TexCoords = vec2(1.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, buildingController[0]);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * tileDiamond[3])); //Top
        TexCoords = vec2(1.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(CIRCLE, buildingController[0]);
        EmitVertex();
        
        EndPrimitive();
        
        
        //Draw the creature after
        
        gl_Position = ortho * view * model * (position + (0.5f * square[0])); //Top right
        TexCoords = vec2(0.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(BUILDING, buildingTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * square[1])); //Bottom right
        TexCoords = vec2(0.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(BUILDING, buildingTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * square[2])); //Top left
        TexCoords = vec2(1.0f, 0.0f);
        TileColor = tileColor[0];
        TexType = ivec2(BUILDING, buildingTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + (0.5f * square[3])); //Bottom left
        TexCoords = vec2(1.0f, 1.0f);
        TileColor = tileColor[0];
        TexType = ivec2(BUILDING, buildingTypeToDraw);
        EmitVertex();
        
        EndPrimitive();
    }
}

void drawDamageBox(vec4 position, int damage, vec4 squre[4]) {
    
    //The positions of a diamond in the shape of the tile
    vec4 tileDiamond[] = vec4[](
        vec4(-0.1f, -0.1f, 0.0f, 0.0f),
        vec4( 0.1f, -0.1f, 0.0f, 0.0f),
        vec4(-0.1f,  0.1f, 0.0f, 0.0f),
        vec4( 0.1f,  0.1f, 0.0f, 0.0f)
    );
    
    vec4 boxCenter = position;
    
    int numberOfDigits = 0;
    
    //Get the number of digits in the damage number
    while ((damage / pow(10, numberOfDigits)) >= 1) {
        numberOfDigits++;
         //The number of digits keeps increasing until the divisor, 10^(num digits), is greater than the damage. This is the number of digits. For example, if the damage is 30, numberOfDigits will keep increasing until 2, where 30 / (10^2) is less than 1. Thus 2 must be the number of digits
    }
    
    //Make the circle stretch if there are more digits
    float horizontalStretch = 0.25f;
    
    if (numberOfDigits > 0)
        horizontalStretch *= numberOfDigits;
    
    gl_Position = ortho * view * model * (boxCenter + (1.0f * tileDiamond[0])); //Bottom
    TexCoords = vec2(0.0f, 0.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (boxCenter + (horizontalStretch * tileDiamond[1])); //Right
    TexCoords = vec2(0.0f, 1.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (boxCenter + (horizontalStretch * tileDiamond[2])); //Left
    TexCoords = vec2(1.0f, 0.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (boxCenter + (1.0f * tileDiamond[3])); //Top
    TexCoords = vec2(1.0f, 1.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    EndPrimitive();
}