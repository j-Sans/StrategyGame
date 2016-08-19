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
    
    vec4 position = gl_in[0].gl_Position;
    
    float top = position.y + (height / 2.0);
    float bottom = position.y - (height / 2.0);
    float left = position.x - (width / 2.0);
    float right = position.x + (width / 2.0);
    
    //The outer box of the bar
    
    gl_Position = vec4(left, top, position.z, position.w);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(left, bottom, position.z, position.w);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(right, top, position.z, position.w);
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(right, bottom, position.z, position.w);
    Color = outsideColor;
    EmitVertex();
    
    EndPrimitive();
    
    //The inner box representing the lost value
    
    float widthInset = width / 10.0;
    float heightInset = height / 10.0;
    
    gl_Position = vec4(left + widthInset, top - heightInset, position.z, position.w);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(left + widthInset, bottom + heightInset, position.z, position.w);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(right - widthInset, top - heightInset, position.z, position.w);
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(right - widthInset, bottom + heightInset, position.z, position.w);
    Color = lostColor;
    EmitVertex();
    
    EndPrimitive();
    
    //The inner box representing the remaining value. This box is stretched an amount proportional to its size
    
    float widthLength = width - (2 * widthInset);
    float heightLength = height - (2 * heightInset);
    
    gl_Position = vec4(left + widthInset, bottom + heightInset + (heightLength /* portionFilled[0]*/), position.z, position.w);
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(left + widthInset, bottom + heightInset, position.z, position.w);
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(left + widthInset + (widthLength /* portionFilled[0]*/), bottom + heightInset + (heightLength /* portionFilled[0]*/), position.z, position.w);
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(left + widthInset + (widthLength /* portionFilled[0]*/), bottom + heightInset, position.z, position.w);
    Color = remainingColor;
    EmitVertex();
    
    EndPrimitive();
}