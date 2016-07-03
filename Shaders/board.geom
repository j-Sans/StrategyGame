#version 330 core

//Preprocessor directives

#define TERRAIN 0
#define CREATURE 1
#define DAMAGE 2

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

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

//4 vertices for tile, 4 vertices for creature, and up to 4 vertices for the damage box (4 * number of digits), for now only 1

in int terrain[];
in int creature[];
in int creatureDirection[];
in int creatureDamage[];
in vec4 tileColor[];
in float creatureOffset[];

out vec2 TexCoords;
out vec4 TileColor;
flat out ivec2 TexType; //First number represents if it is a texture or terrain, and second number represents the respective type

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;
uniform mat4 creatureMat;


//Functions
void makeOpen(vec4 position);
void makeMountain(vec4 position);
void drawCreature(vec4 position, int creatureTypeToDraw, vec4 rect[4]);
void drawDamageBox(vec4 position, int damage, vec4 rect[4]);

void main() {
    vec4 position = gl_in[0].gl_Position;
    
    //Rectangle coordinates with these transformations
    vec4 rect[] = vec4[](
        vec4( 0.45f,  0.35f, 0.0f, 0.0f),
        vec4( 0.05f, -0.05f, 0.0f, 0.0f),
        vec4( 0.35f, 0.45f, 0.0f, 0.0f),
        vec4(-0.05f,  0.05f, 0.0f, 0.0f)
    );
    
    //Draw the ground
    if (terrain[0] == OPEN_TERRAIN) {
        makeOpen(position);
    } else if (terrain[0] == MOUNTAIN_TERRAIN) {
        makeMountain(position);
    }
    
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
            rect[a].y -= creatureOffset[0];
        }
    } else if (creatureDirection[0] == EAST) {
        for (int a = 0; a < 4; a++) {
            rect[a].x += creatureOffset[0];
        }
    } else if (creatureDirection[0] == SOUTH) {
        for (int a = 0; a < 4; a++) {
            rect[a].y += creatureOffset[0];
        }
    } else if (creatureDirection[0] == WEST) {
        for (int a = 0; a < 4; a++) {
            rect[a].x -= creatureOffset[0];
        }
    }
    
    if (creatureTypeToDraw != NO_CREATURE) {
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

void drawDamageBox(vec4 position, int damage, vec4 rect[4]) {
    float digitOffset = 1.0f / 10.0f; //This is the width of one digit in the texture containing all of the digits
    
    gl_Position = ortho * view * model * (position + (0.125f * rect[0])); //Bottom
    TexCoords = vec2(digitOffset + (digitOffset * damage), 0.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + (0.125f * rect[1])); //Right
    TexCoords = vec2(digitOffset + (digitOffset * damage), 1.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + (0.125f * rect[2])); //Left
    TexCoords = vec2(digitOffset * damage, 0.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + (0.125f * rect[3])); //Top
    TexCoords = vec2(digitOffset * damage, 1.0f);
    TileColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TexType = ivec2(DAMAGE, damage);
    EmitVertex();
    
    EndPrimitive();
}