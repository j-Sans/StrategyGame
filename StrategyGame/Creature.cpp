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

Creature::Creature(unsigned int x, unsigned int y, Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, AttackStyle attackStyle,  unsigned int vision, unsigned int range, unsigned int startDirection, unsigned int controller) : Attackable(maxHealth, x, y, controller), raceVal(race), maxEnergyVal(maxEnergy), attackVal(attack), attackStyleVal(attackStyle), visionVal(vision), rangeVal(range) {
    this->healthVal = maxHealth;
    this->energyVal = maxEnergy;
    
    if (startDirection > 3) //Makes sure the direction is valid, otherwise faces north. Can't be less than 0 because it is unsigned.
        this->directionVal = NORTH;
    else
        this->directionVal = startDirection;
}

//Public member functions

bool Creature::takeDamage(unsigned int damage) {
    if (damage >= this->healthVal)
        return true; //The creature has died
    else
        this->healthVal -= damage;
    
    return false; //The creature is still alive
}

void Creature::incrementEnergy(int energy) {
    this->energyVal += energy;
    if (this->energyVal > this->maxEnergyVal) this->energyVal = this->maxEnergyVal;
}

void Creature::decrementEnergy(int energy) {
    if (energy > this->energyVal)
        this->energyVal = 0;
    else if (this->energyVal - energy > this->maxEnergyVal)
        this->energyVal = this->maxEnergyVal;
    else this->energyVal -= energy;
}

void Creature::useAllEnergy() {
    this->energyVal = 0;
}

void Creature::resetEnergy() {
    this->energyVal = this->maxEnergyVal;
}

void Creature::setDirection(unsigned int direction) {
    if (direction <= 3) //If direction is NORTH, EAST, SOUTH, or WEST, because those are up to 3.
        this->directionVal = direction;
}

bool Creature::incrementOffset(float deltaTime) {
    float displacement = Creature::movementAnimationSpeed * deltaTime;
    
    if (this->directionVal == NORTH || this->directionVal == EAST) {
        //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
        
        //If the creature is in the process of moving currently, continue to move it
        if (this->offsetVal > 0.0) {
            this->offsetVal += displacement;
        }
        
        //At 0.4, it has reached the next tile
        if (this->offsetVal > 0.4) {
            this->offsetVal = 0.0;
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Visual: arrived at next tile after moving either North or East\n";
#endif
            
            return true;
        }
    } else if (this->directionVal == SOUTH || this->directionVal == WEST) {
        //These two directions cause the creature to move udown, visually, so they move to the lower tile first. If they moved tiles after, then the new tile, which is lower, would be drawn on top
        
        //The displacement starts at -0.4 and goes towards 0, so it gets closer to 0 as the creature gets closer to the new tile.
        //It starts at -0.4 because because the creature is moved first, but it needs to appear as though it is on the previous tile, which is -0.4 away.
        if (this->offsetVal < 0.0) {
            this->offsetVal += displacement;
            
            if (this->offsetVal > 0.0) {
                this->offsetVal = 0.0;
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
    if (this->directionVal == NORTH || this->directionVal == EAST) {
        this->offsetVal += deltaTime * Creature::movementAnimationSpeed;
    } else if (this->directionVal == SOUTH || this->directionVal == WEST) {
        this->offsetVal = -0.4;
    }
}

void Creature::resetOffset() {
    this->offsetVal = 0.0;
}

void Creature::move(unsigned int direction) {
    if (direction == NORTH) {
        this->yVal--;
    } else if (direction == EAST) {
        this->xVal--;
    } else if (direction == SOUTH) {
        this->yVal++;
    } else if (direction == WEST) {
        this->xVal++;
    }
}

void Creature::setLocation(unsigned int x, unsigned int y) {
    this->xVal = x;
    this->yVal = y;
}

Race Creature::race() const {
    return this->raceVal;
}

std::string Creature::raceString() const {
    if (this->raceVal == Human)
        return "Human";
    else if (this->raceVal == Elf)
        return "Dwarf";
    else if (this->raceVal == Dwarf)
        return "Dwarf";
    else if (this->raceVal == Orc)
        return "Orc";
    else if (this->raceVal == Goblin)
        return "Goblin";
    else if (this->raceVal == Undead)
        return "Undead";
    else if (this->raceVal == Vampire)
        return "Vampire";
    else
        return "Other";
}

unsigned int Creature::maxEnergy() const {
    return this->maxEnergyVal;
}

unsigned int Creature::attack() const {
    return this->attackVal;
}

AttackStyle Creature::attackStyle() const {
    return this->attackStyleVal;
}

unsigned int Creature::vision() const {
    return this->visionVal;
}

unsigned int Creature::range() const {
    return this->rangeVal;
}

bool Creature::melee() const {
    return this->rangeVal > 1 ? false : true;
}

float Creature::energy() const {
    return this->energyVal;
}

int Creature::direction() const {
    return this->directionVal;
}

float Creature::offset() const {
    return this->offsetVal;
}

std::string Creature::serialize() const {
    std::string str = "Creature:" + std::to_string(this->controllerVal) + ",";
    if (this->raceVal == Human) {
        str += "Human,";
    } else if (this->raceVal == Elf) {
        str += "Elf,";
    } else if (this->raceVal == Dwarf) {
        str += "Dwarf,";
    } else if (this->raceVal == Orc) {
        str += "Orc,";
    } else if (this->raceVal == Goblin) {
        str += "Goblin,";
    } else if (this->raceVal == Undead) {
        str += "Undead,";
    } else if (this->raceVal == Vampire) {
        str += "Vampire,";
    } else {
        throw std::invalid_argument("Error serializing creature: unknown creature race");
    }
    if (this->attackStyleVal == Melee) {
        str += "Melee,";
    } else if (this->attackStyleVal == Ranged) {
        str += "Ranged,";
    } else if (this->attackStyleVal == TerrainIgnoring) {
        str += "TerrainIgnoring,";
    } else {
        throw std::invalid_argument("Error serializing creature: unknown creature attack style");
    }
    
    str += std::to_string(maxHealthVal) + "," + std::to_string(maxEnergyVal) + "," + std::to_string(attackVal) + "," + std::to_string(visionVal) + "," + std::to_string(rangeVal) + "," + std::to_string(healthVal) + "," + std::to_string(energyVal) + ",";
    
    if (this->directionVal == NORTH) {
        str += "NORTH,";
    } else if (this->directionVal == EAST) {
        str += "EAST,";
    } else if (this->directionVal == SOUTH) {
        str += "SOUTH,";
    } else if (this->directionVal == WEST) {
        str += "WEST,";
    } else {
        throw std::invalid_argument("Error serializing creature: unknown creature direction");
    }
    
    return str + std::to_string(offsetVal) + "," + std::to_string(xVal) + "," + std::to_string(yVal) + "," + this->Attackable::serialize() + "-Creature-";
    
}

Creature Creature::deserialize(std::string str) {
    str.erase(0, 9); //To erase "Creature:"
    
    Race race = Human;
    AttackStyle attackStyle = Melee;
    int direction;
    
    
    int controller = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    
    //Extract the race of the creature
    
    if (str.substr(0, str.find_first_of(',')) == "Human") {
        race = Human;
    } else if (str.substr(0, str.find_first_of(',')) == "Elf") {
        race = Elf;
    } else if (str.substr(0, str.find_first_of(',')) == "Dwarf") {
        race = Dwarf;
    } else if (str.substr(0, str.find_first_of(',')) == "Orc") {
        race = Orc;
    } else if (str.substr(0, str.find_first_of(',')) == "Goblin") {
        race = Goblin;
    } else if (str.substr(0, str.find_first_of(',')) == "Undead") {
        race = Undead;
    } else if (str.substr(0, str.find_first_of(',')) == "Vampire") {
        race = Vampire;
    } else {
        throw std::invalid_argument("Error deserializing creature: unreadable creature race");
    }
    str = str.substr(str.find_first_of(',') + 1);
    
    if (str.substr(0, str.find_first_of(',')) == "Melee") {
        attackStyle = Melee;
    } else if (str.substr(0, str.find_first_of(',')) == "Ranged") {
        attackStyle = Ranged;
    } else if (str.substr(0, str.find_first_of(',')) == "TerrainIgnoring") {
        attackStyle = TerrainIgnoring;
    } else {
        throw std::invalid_argument("Error deserializing creature: unreadable creature race");
    }
    str = str.substr(str.find_first_of(',') + 1);
    
    //Extract the numerical values of the creature
    int maxHealth = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int maxEnergy = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int attack = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int vision = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int range = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int health = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int energy = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    
    if (str.substr(0, str.find_first_of(',')) == "NORTH") {
        direction = NORTH;
        str = str.substr(str.find_first_of(',') + 1);
    } else if (str.substr(0, str.find_first_of(',')) == "EAST") {
        direction = EAST;
        str = str.substr(str.find_first_of(',') + 1);
    } else if (str.substr(0, str.find_first_of(',')) == "SOUTH") {
        direction = SOUTH;
        str = str.substr(str.find_first_of(',') + 1);
    } else if (str.substr(0, str.find_first_of(',')) == "WEST") {
        direction = WEST;
        str = str.substr(str.find_first_of(',') + 1);
    } else {
        throw std::invalid_argument("Error deserializing creature: unreadable creature direction");
    }
    
    float offset = std::stof(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int x = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int y = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    
    Creature creature(x, y, race, maxHealth, maxEnergy, attack, attackStyle, vision, range, direction, controller);
    creature.takeDamage(maxHealth - health);
    creature.decrementEnergy(maxEnergy - energy);
    creature.setOffset(offset);
    
    creature.buttonInfo = Attackable::deserializeButtons(str.substr(0, str.find_first_of("-Buttons-")));
    
    return creature;
}

void Creature::setOffset(float offset) {
    this->offsetVal = offset;
}
