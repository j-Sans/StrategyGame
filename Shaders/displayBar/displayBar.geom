#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

in float portionFilled; //A float representing the amount the bar is full, as a decimal from 0 to 1

out vec3 Color;

uniform float width;
uniform float height;

uniform vec3 remainingColor;
uniform vec3 lostColor;
uniform vec3 outsideColor;

void main() {
    vec4 position = gl_in[0].gl_Position;
    
    float top = position - (height / 2.0);
    float bottom = position + (height / 2.0);
    float left = position - (width / 2.0);
    float right = position + (width / 2.0);
    
    //Create the outside box so it is drawn first
    
    gl_Position = vec4(bottom, left, 0.0f, 0.0f); //Bottom left
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(top, left, 0.0f, 0.0f); //Top left
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(bottom, right, 0.0f, 0.0f); //Bottom right
    Color = outsideColor;
    EmitVertex();
    
    gl_Position = vec4(top, right, 0.0f, 0.0f); //Top right
    Color = outsideColor;
    EmitVertex();
    
    EndPrimitive();
    
    //Create the background box for lost amount
    
    float widthInset = width / 5.0;
    float heightInset = height / 5.0;
    
    gl_Position = vec4(bottom - heightInset, left + widthInset, 0.0f, 0.0f); //Bottom left
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(top + heightInset, left + widthInset, 0.0f, 0.0f); //Top left
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(bottom - heightInset, right - widthInset, 0.0f, 0.0f); //Bottom right
    Color = lostColor;
    EmitVertex();
    
    gl_Position = vec4(top + heightInset, right - widthInset, 0.0f, 0.0f); //Top right
    Color = lostColor;
    EmitVertex();
    
    EndPrimitive();
    
    //Create the box representing the filled amount
    
    float fillWidth = width - (2 * widthInset);
    float fillHeight = height - (2 * heightInset);
    
    gl_Position = vec4(bottom - heightInset, left + widthInset, 0.0f, 0.0f); //Bottom left
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(top + heightInset, left + widthInset, 0.0f, 0.0f); //Top left
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(bottom - heightInset, (left + widthInset) + (portionFilled * fillWidth), 0.0f, 0.0f); //Bottom left, translated the amount filled
    Color = remainingColor;
    EmitVertex();
    
    gl_Position = vec4(top + heightInset, (left + widthInset) + (portionFilled * fillWidth), 0.0f, 0.0f); //Top left, translated the amount filled
    Color = remainingColor;
    EmitVertex();
    
    EndPrimitive();
}