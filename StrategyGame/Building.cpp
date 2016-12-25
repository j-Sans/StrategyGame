//
//  Building.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Building.hpp"

//Constructor

Building::Building(unsigned int x, unsigned int y, std::string buttonText, std::string action, unsigned int maxHealth, unsigned int cost, unsigned int controller) : buildingX(x), buildingY(y), buildingButtonText(buttonText), buildingAction(action), buildingMaxHealth(maxHealth), buildingCost(cost), buildingController(controller) {
    this->buildingHealth = this->buildingMaxHealth;
}

bool Building::takeDamage(unsigned int damage) {
    if (damage >= this->buildingHealth)
        return true; //The creature has died
    else
        this->buildingHealth -= damage;
    
    return false; //The creature is still alive
}

//Get functions

const std::string Building::buttonText() {
    return this->buildingButtonText;
}

const std::string Building::action() {
    return this->buildingAction;
}

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

const unsigned int Building::controller() {
    return this->buildingController;
}

std::string Building::serialize() {
    return "Building:" + this->buildingButtonText + "," + this->buildingAction + "," + std::to_string(this->buildingMaxHealth) + "," + std::to_string(this->buildingCost) + "," + std::to_string(this->buildingController) + "," + std::to_string(this->buildingX) + "," + std::to_string(this->buildingY) + "," + std::to_string(this->buildingHealth) + ",";
}
