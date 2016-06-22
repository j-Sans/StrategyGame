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
 * @param setMaxHealth An unsigned int representing max health of the creature.
 */
Creature::Creature(unsigned int maxHealth) : creatureMaxHealth(maxHealth) {
    this->creatureHealth = this->creatureMaxHealth;
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