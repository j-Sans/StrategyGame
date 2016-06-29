#version 330 core

//Preprocessor directives

#define TERRAIN 0
#define CREATURE 1

//Terrain
#define OPEN_TERRAIN 0
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

//Creature
#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

in int terrain[];
in int creature[];

out vec2 TexCoords;
flat out ivec2 TexType; //First number represents if it is a texture or terrain, and second number represents the respective type

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;
uniform mat4 creatureMat;


//Functions
void makeOpen(vec4 position);
void makeMountain(vec4 position);
void drawCreature(vec4 position, int creatureTypeToDraw);


void main() {
    vec4 position = gl_in[0].gl_Position;
    
    //Draw the ground
    if (terrain[0] == OPEN_TERRAIN) {
        makeOpen(position);
    } else { //if (terrain[0] == MOUNTAIN_TERRAIN) {
        makeMountain(position);
    }
    
    //Draw present creature
    drawCreature(position, creature[0]);
}

void makeOpen(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    TexCoords = vec2(0.0f, 0.0f);
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    TexCoords = vec2(1.0f, 0.0f);
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    TexCoords = vec2(0.0f, 1.0f);
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    TexCoords = vec2(1.0f, 1.0f);
    TexType = ivec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void makeMountain(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom
    TexCoords = vec2(0.0f, 1.0f);
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Right
    TexCoords = vec2(0.5f, 1.0f);
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Left
    TexCoords = vec2(0.0f, 0.5f);
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.2f,  0.2f, 0.0f, 0.0f)); //Top
    TexCoords = vec2(1.0f, 0.0f);
    TexType = ivec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void drawCreature(vec4 position, int creatureTypeToDraw) {
    if (creatureTypeToDraw != NO_CREATURE) {
        gl_Position = ortho * view * creatureMat * (position + vec4( 0.0f, -0.1f, 0.0f, 0.0f));
        TexCoords = vec2(1.0f, 1.0f);
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * creatureMat * (position + vec4( 0.2f,  0.1f, 0.0f, 0.0f));
        TexCoords = vec2(1.0f, 0.0f);
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * creatureMat* (position + vec4(-0.1f, -0.0f, 0.0f, 0.0f));
        TexCoords = vec2(0.0f, 1.0f);
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * creatureMat * (position + vec4( 0.1f,  0.2f, 0.0f, 0.0f));
        TexCoords = vec2(0.0f, 0.0f);
        TexType = ivec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        EndPrimitive();
    }
}