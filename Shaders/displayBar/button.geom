#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float color[];

uniform float width;
uniform float height;

uniform vec3 remainingColor;
uniform vec3 lostColor;
uniform vec3 outsideColor;

out float Color;

void main() {
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0), (height / 2.0), 0.0f, 0.0f);
    Color = outsideColor[0];
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0), (-height / 2.0), 0.0f, 0.0f);
    Color = outsideColor[0];
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0), (height / 2.0), 0.0f, 0.0f);
    Color = outsideColor[0];
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0), (-height / 2.0), 0.0f, 0.0f);
    Color = outsideColor[0];
    EmitVertex();
    
    EndPrimitive();
}