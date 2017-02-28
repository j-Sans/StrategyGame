//
//  Attackable.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Attackable.hpp"

Attackable::Attackable(unsigned int maxHealth, unsigned int x, unsigned int y, unsigned int controller) : maxHealthVal(maxHealth), healthVal(maxHealth), xVal(x), yVal(y), controllerVal(controller) {}

void Attackable::addButton(std::string action, std::string text){
    this->buttonInfo.push_back({action, text});
}

void Attackable::removeButtonByAction(std::string action) {
    for (auto a = this->buttonInfo.begin(); a != this->buttonInfo.end(); a++) {
        if (a->first == action) {
            a = this->buttonInfo.erase(a);
        }
    }
}

void Attackable::removeButtonByText(std::string text) {
    for (auto a = this->buttonInfo.begin(); a != this->buttonInfo.end(); a++) {
        if (a->second == text) {
            a = this->buttonInfo.erase(a);
        }
    }
}

void Attackable::removeFirstButton() {
    this->buttonInfo.pop_front();
}

void Attackable::removeLastButton() {
    this->buttonInfo.pop_back();
}

unsigned int Attackable::maxHealth() const {
    return this->maxHealthVal;
}

unsigned int Attackable::health() const {
    return this->healthVal;
}

unsigned int Attackable::x() const {
    return this->xVal;
}

unsigned int Attackable::y() const {
    return this->yVal;
}

unsigned int Attackable::controller() const {
    return this->controllerVal;
}

std::string Attackable::serialize() const {
    std::string str = "Buttons:";
    for (auto a = this->buttonInfo.begin(); a != this->buttonInfo.end(); a++) {
        str += "<" + a->first + "," + a->second + ">";
    }
    return str + "-Buttons-";
}

std::list<std::pair<std::string, std::string> > Attackable::deserializeButtons(std::string str) {
    str.erase(0, 8); //Erases "Buttons:"
    
    std::list<std::pair<std::string, std::string> > buttonInfo;
    
    while (str.find(">") != std::string::npos) {
        str.erase(0, 1); //Erases "<"
        std::string action = str.substr(0, str.find_first_of(','));
        str = str.substr(str.find_first_of(',') + 1);
        std::string text = str.substr(0, str.find_first_of('>'));
        str = str.substr(str.find_first_of('>') + 1);
        buttonInfo.push_back({action, text});
    }
    
    return buttonInfo;
}
