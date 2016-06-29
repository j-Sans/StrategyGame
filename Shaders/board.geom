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

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in int terrain[];
in int creature[];

out vec2 TexCoords; //First two are coordinates, third says which texture
out vec2 TexType;

//out ivec2 TexType;

uniform mat4 model;
uniform mat4 view;
uniform mat4 ortho;


//Functions
void makeOpen(vec4 position);
void makeMountain(vec4 position);
void drawCreature(vec4 position, int creatureTypeToDraw);


void main() {
    //Draw the ground
    if (terrain[0] == OPEN_TERRAIN) {
        makeOpen(gl_in[0].gl_Position);
    } else if (terrain[0] == MOUNTAIN_TERRAIN) {
        makeMountain(gl_in[0].gl_Position);
    }
    
    //Draw present creature
//    drawCreature(gl_in[0].gl_Position, creature[0]);
}

void makeOpen(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    TexCoords = vec2(0.0f, 0.0f);
    TexType = vec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    TexCoords = vec2(1.0f, 0.0f);
    TexType = vec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    TexCoords = vec2(0.0f, 1.0f);
    TexType = vec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    TexCoords = vec2(1.0f, 1.0f);
    TexType = vec2(TERRAIN, OPEN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void makeMountain(vec4 position) {
    gl_Position = ortho * view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom
    TexCoords = vec2(0.0f, 1.0f);
    TexType = vec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Right
    TexCoords = vec2(0.5f, 1.0f);
    TexType = vec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Left
    TexCoords = vec2(0.0f, 0.5f);
    TexType = vec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = ortho * view * model * (position + vec4( 0.2f,  0.2f, 0.0f, 0.0f)); //Top
    TexCoords = vec2(1.0f, 0.0f);
    TexType = vec2(TERRAIN, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void drawCreature(vec4 position, int creatureTypeToDraw) {
    if (creatureTypeToDraw != NO_CREATURE) {
        gl_Position = ortho * view * model * (position + vec4(-0.1f,  0.0f, 0.0f, 0.0f));
        TexCoords = vec2(0.0f, 0.0f);
        TexType = vec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + vec4( 0.0f, -0.1f, 0.0f, 0.0f));
        TexCoords = vec2(1.0f, 0.0f);
        TexType = vec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + vec4( 0.1f,  0.0f, 0.0f, 0.0f));
        TexCoords = vec2(0.0f, 1.0f);
        TexType = vec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        gl_Position = ortho * view * model * (position + vec4( 0.0f,  0.1f, 0.0f, 0.0f));
        TexCoords = vec2(1.0f, 1.0f);
        TexType = vec2(CREATURE, creatureTypeToDraw);
        EmitVertex();
        
        EndPrimitive();
    }
}