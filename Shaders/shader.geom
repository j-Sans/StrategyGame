#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

uniform mat4 model;

void makeSquare(vec4 position) {
    fColor = gs_in[0].color; //Make the color the one received in the first (and only) index in the interface blocks
    
    
    gl_Position = model * (position + vec4(-0.1f, -0.1f, 0.0f, 0.0f)); //Bottom left
    EmitVertex();
    gl_Position = model * (position + vec4( 0.1f, -0.1f, 0.0f, 0.0f)); //Bottom right
    EmitVertex();
    gl_Position = model * (position + vec4(-0.1f,  0.1f, 0.0f, 0.0f)); //Top left
    EmitVertex();
    gl_Position = model * (position + vec4( 0.1f,  0.1f, 0.0f, 0.0f)); //Top right
    EmitVertex();
    
    EndPrimitive();
}

void main() {
    makeSquare(gl_in[0].gl_Position);
}