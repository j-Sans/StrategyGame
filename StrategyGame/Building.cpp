//
//  Building.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Building.hpp"

//Constructor

Building::Building(unsigned int x, unsigned int y, unsigned int maxHealth, unsigned int controller) : Attackable(maxHealth, x, y, controller) {
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

const std::string Building::serialize() {
    return "Building:" + std::to_string(this->maxHealthVal) + "," + std::to_string(this->controllerVal) + "," + std::to_string(this->xVal) + "," + std::to_string(this->yVal) + "," + std::to_string(this->healthVal) + "," + this->Attackable::serialize() + "-Building-";
}

Building Building::deserialize(std::string str) {
    str.erase(0, 9); //To erase "Building:"
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
    
    Building building(x, y, maxHealth, controller);
    building.takeDamage(maxHealth - health);
    
    building.buttonInfo = Attackable::deserializeButtons(str.substr(0, str.find_first_of("-Buttons-")));
    
    return building;
}
