#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;

void makeSquare(vec4 position) {
    
    gl_Position = view * model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    TexCoords = vec2(0.0f, 0.0f);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    TexCoords = vec2(1.0f, 0.0f);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    TexCoords = vec2(0.0f, 1.0f);
    EmitVertex();
    
    gl_Position = view * model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    TexCoords = vec2(1.0f, 1.0f);
    EmitVertex();
    
    EndPrimitive();
}

void main() {
    makeSquare(gl_in[0].gl_Position);
}