//
//  Creature.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Creature.hpp"

//Constructors

/** Creates a creature object.
 *
 * @param race A Race enum type respresenting the creature's race. Includes: Human, Elf, Dwarf, Orc, Goblin, Undead, and Vampire.
 * @param maxHealth An unsigned int representing the max health of the creature.
 * @param maxEnergy An unsigned int representing the max energy of the creature.
 * @param attack An unsigned int representing the damage value the creature.
 * @param speed An unsigned int representing the movement rate per turn of the creature.
 * @param vision An unsigned int representing the radius of squares around the creature that it can reveal.
 * @param range An unsigned int representing the distance the creature can attack.
 * @param startDirection A Direction enum type representing the initial direction that the creature faces. Includes: North, East, South, and West.
 */
Creature::Creature(Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, unsigned int speed, unsigned int vision, unsigned int range, Direction startDirection) : creatureRace(race), creatureMaxHealth(maxHealth), creatureMaxEnergy(maxEnergy), creatureAttack(attack), creatureSpeed(speed), creatureVision(vision), creatureRange(range) {
    this->creatureHealth = maxHealth;
    this->creatureEnergy = maxEnergy;
    this->creatureDirection = startDirection;
}

//Public member functions

/** Reduces the creature's health by the damage argument, and if the creature dies, returns true.
 *
 * @param damage An unsigned int representing damage this creature takes.
 *
 * @return True if the creature dies, otherwise false.
 */
bool Creature::takeDamage(unsigned int damage) {
    if (damage >= this->creatureHealth)
        return true; //The creature has died
    else
        this->creatureHealth -= damage;
    
    return false; //The creature is still alive
}