//
//  Creature.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Creature.hpp"

//Constructors

Creature::Creature(Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, unsigned int vision, unsigned int range, unsigned int cost, unsigned int startDirection, unsigned int controller) : creatureRace(race), creatureMaxHealth(maxHealth), creatureMaxEnergy(maxEnergy), creatureAttack(attack), creatureVision(vision), creatureRange(range), creatureCost(cost), creatureController(controller) {
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

void Creature::incrementOffset(float deltaTime) {
    float displacement = this->movementAnimationSpeed * deltaTime;
    
    if (this->creatureDirection == NORTH || this->creatureDirection == EAST) {
        //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
        
        //If the creature is in the process of moving currently, continue to move it
        if (this->creatureOffset > 0.0) {
            this->creatureOffset += displacement;
        }
        
        //At 0.4, it has reached the next tile
        if (this->creatureOffset > 0.4) {
            this->creatureOffset = 0.0;
            
            this->shouldMove = true;
        }
    } else if (this->creatureDirection == SOUTH || this->creatureDirection == WEST) {
        //These two directions cause the creature to move udown, visually, so they move to the lower tile first. If they moved tiles after, then the new tile, which is lower, would be drawn on top
        
        //The displacement starts at -0.4 and goes towards 0, so it gets closer to 0 as the creature gets closer to the new tile.
        //It starts at -0.4 because because the creature is moved first, but it needs to appear as though it is on the previous tile, which is -0.4 away.
        if (this->creatureOffset < 0.0) {
            this->creatureOffset += displacement;
            
            if (this->creatureOffset > 0.0) {
                this->creatureOffset = 0.0;
            }
        }
        
        //At 0.0, it has reached the next tile
        if (this->creatureOffset > 0.0) {
            
            this->creatureOffset = 0.0;
            
            //The creature is not moved here. It should have already been moved when the offset was initially changed. For that reason, shouldMove is not set to true
        }
    }
}

void Creature::initiateOffsetForMovingDown() {
    if (this->creatureDirection == SOUTH || this->creatureDirection == WEST) {
        this->creatureOffset = -0.4;
    }
}

void Creature::resetOffset() {
    this->creatureOffset = 0.0;
}

const Race Creature::race() {
    return this->creatureRace;
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

const unsigned int Creature::vision() {
    return this->creatureVision;
}

const unsigned int Creature::range() {
    return this->creatureRange;
}

const unsigned int Creature::cost() {
    return this->creatureCost;
}

const bool Creature::melee() {
    return this->creatureRange > 1 ? false : true;
}

unsigned int Creature::health() {
    return this->creatureHealth;
}

unsigned int Creature::energy() {
    return this->creatureEnergy;
}

int Creature::direction() {
    return this->creatureDirection;
}

float Creature::offset() {
    return this->creatureOffset;
}

bool Creature::readyToMove() {
    if (this->shouldMove) {
        this->shouldMove = false;
        return true;
    } else
        return false;
}

const unsigned int Creature::controller() {
    return this->creatureController;
}