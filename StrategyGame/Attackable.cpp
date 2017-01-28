//
//  Attackable.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Attackable.hpp"

unsigned int Attackable::maxHealth() {
    return this->maxHealthVal;
}

unsigned int Attackable::health() {
    return this->buildingHealth;
}

unsigned int Attackable::x() {
    return this->xVal;
}

unsigned int Attackable::y() {
    return this->yVal;
}

unsigned int Attackable::controller() {
    return this->controllerVal;
}
