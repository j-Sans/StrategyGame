//
//  Building.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Building.hpp"

//Constructor

Building::Building(unsigned int x, unsigned int y, unsigned int maxHealth, unsigned int cost) : buildingX(x), buildingY(y), buildingMaxHealth(maxHealth), buildingCost(cost) {
    this->buildingHealth = this->buildingMaxHealth;
}

//Get functions

const unsigned int Building::maxHealth() {
    return this->buildingMaxHealth;
}

unsigned int Building::health() {
    return this->buildingHealth;
}

const unsigned int Building::cost() {
    return this->buildingCost;
}

const unsigned int Building::x() {
    return this->buildingX;
}

const unsigned int Building::y() {
    return this->buildingY;
}