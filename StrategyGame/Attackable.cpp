//
//  Attackable.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Attackable.hpp"

Attackable::Attackable(unsigned int maxHealth, unsigned int x, unsigned int y, unsigned int controller) : maxHealthVal(maxHealth), healthVal(maxHealth), xVal(x), yVal(y), controllerVal(controller) {}

unsigned int Attackable::maxHealth() {
    return this->maxHealthVal;
}

unsigned int Attackable::health() {
    return this->healthVal;
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
