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

//Sets a creature as the creature located in this tile. Warning: Does not deletes the old creature!
void Tile::setCreature(Creature *creature) {
    
    this->tileCreature = creature; //Set the creature at this tile as the inputted creature
}

void Tile::setStyle(Style style) {
    this->tileStyle = style;
    
    if (style == Regular)
        this->tileColor = White;
    else if (style == Selected)
        this->tileColor = Tile::selectedTileColor;
    else if (style == OpenAdj)
        this->tileColor = Tile::openAdjTileColor;
    else if (style == AttackableAdj)
        this->tileColor = Tile::attackableAdjTileColor;
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
        return glm::vec3(0.625f, 0.625f, 0.625f);
    else if (this->tileColor == Red)
        return glm::vec3(1.0f, 0.625f, 0.625f);
    else if (this->tileColor == Yellow)
        return glm::vec3(1.0f, 1.0f, 0.625f);
    else if (this->tileColor == Green)
        return glm::vec3(0.62f, 1.0f, 0.625f);
    else if (this->tileColor == Cyan)
        return glm::vec3(0.625f, 1.0f, 1.0f);
    else if (this->tileColor == Blue)
        return glm::vec3(0.625f, 0.625f, 1.0f);
    
    //Something went wrong. Return White to have an unaltered color
    return glm::vec3(1.0f, 1.0f, 1.0f);
}

Style Tile::style() {
    return this->tileStyle;
}

bool Tile::passableByCreature(Creature creature) {
    //Mountains are not passable (except by dwarves)
    if (this->tileTerrain == MOUNTAIN_TERRAIN && creature.race() != Dwarf) {
        return false;
    }
    
    //Water is never passable
    if (this->tileTerrain == WATER_TERRAIN) {
        return false;
    }
    
    //Tiles with creatures are never passable
    if (this->occupied()) {
        return false;
    }
    
    //Everything else is passable
    return true;
}