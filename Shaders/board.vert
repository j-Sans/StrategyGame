#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in int terrainInt;
layout (location = 2) in ivec3 creatureInt;
layout (location = 3) in vec3 tileColorIn;
layout (location = 4) in int damageInt;
layout (location = 5) in float offset;

out int terrain;
out int creature;
out int creatureDirection;
out int creatureController;
out int creatureDamage;
out vec4 tileColor;
out float creatureOffset;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    terrain = terrainInt;
    creature = creatureInt.x;
    creatureDirection = creatureInt.y;
    creatureController = creatureInt.z;
    creatureDamage = damageInt;
    tileColor = vec4(tileColorIn, 1.0f);
    creatureOffset = offset;
}