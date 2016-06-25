#version 330 core

//Preprocessor directives
#define OPEN_TERRAIN 0.0f
#define MOUNTAIN_TERRAIN 1.0f

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float terrain[];

out vec3 TexCoords; //First two are coordinates, third says which texture

uniform mat4 model;
uniform mat4 view;

void makeSquare(vec4 position) {
    gl_Position = view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    TexCoords = vec3(0.0f, 0.0f, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    TexCoords = vec3(1.0f, 0.0f, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    TexCoords = vec3(0.0f, 1.0f, OPEN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    TexCoords = vec3(1.0f, 1.0f, OPEN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}

void makeMountain(vec4 position) {
    gl_Position = view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom
    TexCoords = vec3(0.0f, 1.0f, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Right
    TexCoords = vec3(0.5f, 1.0f, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Left
    TexCoords = vec3(0.0f, 0.5f, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.2f,  0.2f, 0.0f, 0.0f)); //Top
    TexCoords = vec3(1.0f, 0.0f, MOUNTAIN_TERRAIN);
    EmitVertex();
    
    EndPrimitive();
}


void main() {
    if (terrain[0] == OPEN_TERRAIN) {
        makeSquare(gl_in[0].gl_Position);
    } else {
        makeMountain(gl_in[0].gl_Position);
    }
}