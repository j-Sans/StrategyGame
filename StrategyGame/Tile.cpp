//
//  Tile.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Tile.hpp"

//Constructors
Tile::Tile(int terrain, unsigned int x, unsigned int y) : tileX(x), tileY(y) {
    this->tileTerrain = terrain;
}

//Destructor

//Public member functions

//Sets a creature as the creature located in this tile. Warning: Does not delete the old creature!
void Tile::setCreature(Creature *creature) {
    this->tileCreature = creature; //Set the creature at this tile as the inputted creature
}

//Sets a building as the building located in this tile. Warning: Does not delete the old building!
void Tile::setBuilding(Building *building) {
    this->tileBuilding = building; //Set the building at this tile as the inputted creature
}

void Tile::setDirection(unsigned int direction) {
    if (this->tileCreature != nullptr && direction <= 3) { //As long as there is a creature and the direction is valid
        this->tileCreature->setDirection(direction);
    }
}

void Tile::setDamage(unsigned int damage, float time) {
    this->tileDamage = damage;
    this->damageHitTime = time;
}

unsigned int Tile::x() const {
    return this->tileX;
}

unsigned int Tile::y() const {
    return this->tileY;
}

Creature* Tile::creature() const {
    return this->tileCreature;
}

Building* Tile::building() const {
    return this->tileBuilding;
}

unsigned int Tile::terrain() const {
    return this->tileTerrain;
}

bool Tile::occupied() const {
    return this->tileCreature != nullptr ? true : this->tileBuilding ? true : false;
}

//Currently this returns the basic stick figure creature whenever there is any creature.
unsigned int Tile::creatureType() const {
    if (this->tileCreature == nullptr) {
        return NO_CREATURE;
    } else {
        return STICK_FIGURE_CREATURE;
    }
}

//Currently this returns the basic tower building whenever there is any building.
unsigned int Tile::buildingType() const {
    if (this->tileBuilding == nullptr) {
        return NO_BUILDING;
    } else {
        return TOWER_BUILDING;
    }
}

bool Tile::passableByCreature(const Creature& creature) const {
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

bool Tile::passableByAttackStyle(const Creature& creature) const {
    //Mountains are not passable (except by terrain ignoring spells)
    if (this->tileTerrain == MOUNTAIN_TERRAIN) {
        if (creature.attackStyle() != TerrainIgnoring) {
            return false;
        }
    }
    
    //Water is not passable by melee attacks.
    if (this->tileTerrain == WATER_TERRAIN) {
        if (creature.attackStyle() == Melee) {
            return false;
        }
    }
    
    /*
    //Tiles with large creatures are never passable
    if (this->creature.creatureCharacteristics {includes blocking}  ()) {
        return false;
    }
     
     //not sure here whether to have creature characteristics as a string or array or to have booleans of isFlying, isBlocking, isAmphibious, etc.
     
    */
    
    //Everything else is passable
    return true;
}

bool Tile::passableByVision(const Creature& creature) const {
    //This function will be used for later functionality. DO NOT REMOVE.
    return true;
}

unsigned int Tile::damage() const {
    return this->tileDamage;
}

float Tile::timeOfDamage() const {
    return this->damageHitTime;
}

std::string Tile::serialize() const {
    return "Tile:" + std::to_string(this->tileX) + "," + std::to_string(this->tileY) + "," + std::to_string(this->tileTerrain) + "," + std::to_string(this->tileDamage) + "," + std::to_string(this->damageHitTime) + "-Tile-";
}

Tile Tile::deserialize(std::string str) {
    str.erase(0, 5); //To erase "Tile:"
    std::string num = str.substr(0, str.find_first_of(','));
    int x = std::stoi(num);
    str = str.substr(str.find_first_of(',') + 1);
    int y = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int terrain = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int damage = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    float damageTime = std::stof(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    
    Tile tile(terrain, x, y);
    tile.setDamage(damage, damageTime);
    return tile;
}
