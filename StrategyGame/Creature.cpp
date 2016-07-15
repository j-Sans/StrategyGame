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
    return this->creatureAttack; }

//Do we need this? Isn't speed just based on the amount of movement points the unit has?

//const unsigned int Creature::speed() {
//    return this->creatureSpeed;
//}

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

const unsigned int Creature::controller() {
    return this->creatureController;
}