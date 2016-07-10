//
//  Building.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Building.hpp"

const unsigned int Building::maxHealth() {
    return this->buildingMaxHealth;
}

unsigned int Building::health() {
    return this->buildingHealth;
}

const unsigned int Building::cost() {
    return this->buildingCost;
}