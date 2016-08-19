#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

in float portionFilled[];

uniform float width;
uniform float height;

uniform vec3 remainingColor;
uniform vec3 lostColor;
uniform vec3 outsideColor;

out vec3 Color;

void main() {
    
    //The outer box of the bar
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0), (height / 2.0), 0.0f, 0.0f);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0), (-height / 2.0), 0.0f, 0.0f);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0), (height / 2.0), 0.0f, 0.0f);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0), (-height / 2.0), 0.0f, 0.0f);
    Color = outsideColor;
    EmitVertex();
    
    EndPrimitive();
    
    //The inner box representing the lost value
    
    float widthInset = width / 10.0;
    float heightInset = height / 10.0;
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0) - widthInset, (height / 2.0) - heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((width / 2.0) - widthInset, (-height / 2.0) + heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset, (height / 2.0) - heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset, (-height / 2.0) + heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    EndPrimitive();
    
    //The inner box representing the remaining value. This box is stretched an amount proportional to its size
    
    float widthLength = width - (width * 2 / 10.0);
    float heightLength = height - (height * 2 / 10.0);
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset + (widthLength * portionFilled[0]), (height / 2.0) - heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset + (widthLength * portionFilled[0]), (-height / 2.0) + heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset, (height / 2.0) - heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4((- width / 2.0) + widthInset, (-height / 2.0) + heightInset, 0.0f, 0.0f);
    Color = lostColor;
    EmitVertex();
    
    EndPrimitive();
}