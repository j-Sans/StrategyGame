//
//  Creature.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Creature.hpp"
#include <iostream>

//Constructors

Creature::Creature(unsigned int x, unsigned int y, Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, AttackStyle attackStyle,  unsigned int vision, unsigned int range, unsigned int startDirection, unsigned int controller) : creatureX(x), creatureY(y), creatureRace(race), creatureMaxHealth(maxHealth), creatureMaxEnergy(maxEnergy), creatureAttack(attack), creatureAttackStyle(attackStyle), creatureVision(vision), creatureRange(range),  creatureController(controller) {
    this->creatureHealth = maxHealth;
    this->creatureEnergy = maxEnergy;
    
    if (startDirection > 3) //Makes sure the direction is valid, otherwise faces north. Can't be less than 0 because it is unsigned.
        this->creatureDirection = NORTH;
    else
        this->creatureDirection = startDirection;
}

//Public member functions

bool Creature::takeDamage(unsigned int damage) {
    if (damage >= this->creatureHealth)
        return true; //The creature has died
    else
        this->creatureHealth -= damage;
    
    return false; //The creature is still alive
}

void Creature::decrementEnergy(unsigned int energy) {
    if (energy < this->creatureEnergy)
        this->creatureEnergy -= energy;
    else
        this->creatureEnergy = 0;
}

void Creature::useAllEnergy() {
    this->creatureEnergy = 0;
}

void Creature::resetEnergy() {
    this->creatureEnergy = this->creatureMaxEnergy;
}

void Creature::setDirection(unsigned int direction) {
    if (direction <= 3) //If direction is NORTH, EAST, SOUTH, or WEST, because those are up to 3.
        this->creatureDirection = direction;
}

bool Creature::incrementOffset(float deltaTime) {
    float displacement = Creature::movementAnimationSpeed * deltaTime;
    
    if (this->creatureDirection == NORTH || this->creatureDirection == EAST) {
        //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
        
        //If the creature is in the process of moving currently, continue to move it
        if (this->creatureOffset > 0.0) {
            this->creatureOffset += displacement;
        }
        
        //At 0.4, it has reached the next tile
        if (this->creatureOffset > 0.4) {
            this->creatureOffset = 0.0;
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Visual: arrived at next tile after moving either North or East\n";
#endif
            
            return true;
        }
    } else if (this->creatureDirection == SOUTH || this->creatureDirection == WEST) {
        //These two directions cause the creature to move udown, visually, so they move to the lower tile first. If they moved tiles after, then the new tile, which is lower, would be drawn on top
        
        //The displacement starts at -0.4 and goes towards 0, so it gets closer to 0 as the creature gets closer to the new tile.
        //It starts at -0.4 because because the creature is moved first, but it needs to appear as though it is on the previous tile, which is -0.4 away.
        if (this->creatureOffset < 0.0) {
            this->creatureOffset += displacement;
            
            if (this->creatureOffset > 0.0) {
                this->creatureOffset = 0.0;
#ifdef MOVEMENT_CONSOLE_OUTPUT
                std::cout << "Visual: arrived at next tile after moving either South or West\n";
#endif
                
                return true;
                
                 //The creature is not moved here. It should have already been moved when the offset was initially changed
            }
        }
    }
    
    return false;
}

void Creature::initiateMovementOffset(float deltaTime) {
    if (this->creatureDirection == NORTH || this->creatureDirection == EAST) {
        this->creatureOffset += deltaTime * Creature::movementAnimationSpeed;
    } else if (this->creatureDirection == SOUTH || this->creatureDirection == WEST) {
        this->creatureOffset = -0.4;
    }
}

void Creature::resetOffset() {
    this->creatureOffset = 0.0;
}

void Creature::move(unsigned int direction) {
    if (direction == NORTH) {
        this->creatureY--;
    } else if (direction == EAST) {
        this->creatureX--;
    } else if (direction == SOUTH) {
        this->creatureY++;
    } else if (direction == WEST) {
        this->creatureX++;
    }
}

void Creature::setLocation(unsigned int x, unsigned int y) {
    this->creatureX = x;
    this->creatureY = y;
}

const Race Creature::race() {
    return this->creatureRace;
}

std::string Creature::raceString() {
    if (this->creatureRace == Human)
        return "Human";
    else if (this->creatureRace == Elf)
        return "Dwarf";
    else if (this->creatureRace == Dwarf)
        return "Dwarf";
    else if (this->creatureRace == Orc)
        return "Orc";
    else if (this->creatureRace == Goblin)
        return "Goblin";
    else if (this->creatureRace == Undead)
        return "Undead";
    else if (this->creatureRace == Vampire)
        return "Vampire";
    else
        return "Other";
}

const unsigned int Creature::maxHealth() {
    return this->creatureMaxHealth;
}

const unsigned int Creature::maxEnergy() {
    return this->creatureMaxEnergy;
}

const unsigned int Creature::attack() {
    return this->creatureAttack;
}

const AttackStyle Creature::attackStyle() {
    return this->creatureAttackStyle;
}

const unsigned int Creature::vision() {
    return this->creatureVision;
}

const unsigned int Creature::range() {
    return this->creatureRange;
}

const bool Creature::melee() {
    return this->creatureRange > 1 ? false : true;
}

unsigned int Creature::health() {
    return this->creatureHealth;
}

float Creature::energy() {
    return this->creatureEnergy;
}

int Creature::direction() {
    return this->creatureDirection;
}

float Creature::offset() {
    return this->creatureOffset;
}

const unsigned int Creature::controller() {
    return this->creatureController;
}

unsigned int Creature::x() {
    return this->creatureX;
}

unsigned int Creature::y() {
    return this->creatureY;
}

std::string Creature::serialize() {
    std::string str = "Creature:" + std::to_string(this->creatureController) + ",";
    if (this->creatureRace == Human) {
        str += "Human,";
    } else if (this->creatureRace == Elf) {
        str += "Elf,"
    } else if (this->creatureRace == Dwarf) {
        str += "Dwarf,";
    } else if (this->creatureRace == Orc) == 0) {
        str += "Orc,";
    } else if (this->creatureRace == Goblin) == 0) {
        str += "Goblin,";
    } else if (this->creatureRace == Undead) == 0) {
        str += "Undead,";
    } else if (this->creatureRace == Vampire) == 0) {
        str += "Vampire,";
    } else {
        throw std::invalid_argument("Error serializing creature: unknown creature race");
    }
    if (this->creatureAttackStyle == Melee) {
        str += "Melee,";
    } else if (this->creatureRace == Ranged) {
        str += "Ranged,"
    } else if (this->creatureRace == TerrainIgnoring) == 0) {
        str += "TerrainIgnoring,";
    } else {
        throw std::invalid_argument("Error serializing creature: unknown creature attack style");
    }
    
    return str + std::to_string(creatureMaxHealth) + "," + std::to_string(creatureMaxEnergy) + "," + std::to_string(creatureAttack) + "," + std::to_string(creatureVision) + "," + std::to_string(creatureRange) + "," + std::to_string;
    
}


const unsigned int creatureRange;

unsigned int creatureHealth;
unsigned int creatureEnergy;
int creatureDirection;
float creatureOffset = 0.0;

//Location
unsigned int creatureX;
unsigned int creatureY;
