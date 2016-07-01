//
//  Tile.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Tile.hpp"

//Constructors
Tile::Tile(float terrain, unsigned int x, unsigned int y) : tileX(x), tileY(y) {
    this->tileTerrain = terrain;
}

//Destructor

//Public member functions

//Sets a creature as the creature located in this tile. Deletes the old creature to avoid memory leaks, and adjusts whether the tile is occupied.
void Tile::setCreature(Creature *creature) {
    if (this->tileCreature != nullptr)
        delete this->tileCreature; //Prevent memory leaks
    
    this->tileCreature = creature; //Set the creature at this tile as the inputted creature
}

void Tile::setColor(Color color) {
    this->tileColor = color;
}

const unsigned int Tile::x() {
    return this->tileX;
}

const unsigned int Tile::y() {
    return this->tileY;
}

Creature* Tile::creature() {
    return this->tileCreature;
}

float Tile::terrain() {
    return this->tileTerrain;
}

bool Tile::occupied() {
    return this->tileCreature != nullptr ? true : false;
}

//Currently this returns the basic stick figure creature whenever there is any creature.
unsigned int Tile::creatureType() {
    if (this->tileCreature == nullptr) {
        return NO_CREATURE;
    } else {
        return STICK_FIGURE_CREATURE;
    }
}

Color Tile::colorName() {
    return this->tileColor;
}

glm::vec3 Tile::color() {
    if (this->tileColor == White)
        return glm::vec3(1.0f, 1.0f, 1.0f);
    else if (this->tileColor == Grey)
        return glm::vec3(0.67f, 0.67f, 0.67f);
    else if (this->tileColor == Red)
        return glm::vec3(1.0f, 0.0f, 0.0f);
    else if (this->tileColor == Yellow)
        return glm::vec3(1.0f, 1.0f, 0.0f);
    else if (this->tileColor == Green)
        return glm::vec3(0.0f, 1.0f, 0.0f);
    else if (this->tileColor == Blue)
        return glm::vec3(0.0f, 0.0f, 1.0f);
    
    //Something went wrong. Return White to have an unaltered color
    return glm::vec3(1.0f, 1.0f, 1.0f);
}