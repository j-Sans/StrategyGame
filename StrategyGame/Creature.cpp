//
//  Creature.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Creature.hpp"

//Constructors

Creature::Creature(Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, unsigned int speed, unsigned int vision, unsigned int range, unsigned int cost, Direction startDirection) : creatureRace(race), creatureMaxHealth(maxHealth), creatureMaxEnergy(maxEnergy), creatureAttack(attack), creatureSpeed(speed), creatureVision(vision), creatureRange(range), creatureCost(cost) {
    this->creatureHealth = maxHealth;
    this->creatureEnergy = maxEnergy;
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

const unsigned int Creature::speed() {
    return this->creatureSpeed;
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

unsigned int Creature::health() {
    return this->creatureHealth;
}

unsigned int Creature::energy() {
    return this->creatureEnergy;
}

Direction Creature::direction() {
    return this->creatureDirection;
}

bool Creature::hasMeleeAttack() {
    return this->hasMeleeAttack;
}