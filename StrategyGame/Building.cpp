//
//  Building.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Building.hpp"

//Constructor

Building::Building(unsigned int x, unsigned int y, std::string buttonText, std::string action, unsigned int maxHealth, unsigned int controller) : Attackable(maxHealth, x, y, controller), buildingButtonText(buttonText), buildingAction(action) {
    this->healthVal = this->maxHealthVal;
}

bool Building::takeDamage(unsigned int damage) {
    if (damage >= this->healthVal)
        return true; //The creature has died
    else
        this->healthVal -= damage;
    
    return false; //The creature is still alive
}

//Get functions

const std::string Building::buttonText() {
    return this->buildingButtonText;
}

const std::string Building::action() {
    return this->buildingAction;
}

std::string Building::serialize() {
    return "Building:" + this->buildingButtonText + "," + this->buildingAction + "," + std::to_string(this->maxHealthVal) + "," + std::to_string(this->controllerVal) + "," + std::to_string(this->xVal) + "," + std::to_string(this->yVal) + "," + std::to_string(this->healthVal) + "-Building-";
}

Building Building::deserialize(std::string str) {
    str.erase(0, 9); //To erase "Building:"
    std::string buttonText = str.substr(0, str.find_first_of(','));
    str = str.substr(str.find_first_of(',') + 1);
    std::string action = str.substr(0, str.find_first_of(','));
    str = str.substr(str.find_first_of(',') + 1);
    int maxHealth = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int controller = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int x = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int y = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    int health = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    
    Building building(x, y, buttonText, action, maxHealth, controller);
    building.takeDamage(maxHealth - health);
    
    return building;
}
