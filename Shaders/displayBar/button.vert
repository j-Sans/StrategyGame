#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in float partFilled;

out float portionFilled; //A float representing the amount the bar is full, as a decimal from 0 to 1

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    portionFilled = partFilled;
}