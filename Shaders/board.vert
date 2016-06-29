#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in int terrainInt;
layout (location = 2) in int creatureInt;

out int terrain;
out int creature;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    terrain = terrainInt;
    creature = creatureInt;
}