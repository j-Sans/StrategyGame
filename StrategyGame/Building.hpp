//
//  Building.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Building_hpp
#define Building_hpp

#include "Macros.h"
#include "Attackable.hpp"

//Standard library includes
#include <string>

class Building : Attackable {
public:
    //Constructor
    
    Building(unsigned int x, unsigned int y, std::string buttonText, std::string action, unsigned int maxHealth, unsigned int controller);
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    
    /*!
     * Reduces the creature's health by the damage argument, and if the creature dies, returns true.
     *
     * @param damage An unsigned int representing damage this creature takes.
     *
     * @return True if the creature dies, otherwise false.
     */
    bool takeDamage(unsigned int damage);
    
    //Public get functions
    
    /*!
     * @return The text string for the button on the building to be used on the building interface.
     */
    const std::string buttonText();
    
    /*!
     * @return The action string for the button on the building to be used on the building interface.
     */
    const std::string action();
    
    /*!
     * Serialize this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    std::string serialize();
    
    /*!
     * Create an object from a serialized string that can be sent through sockets.
     *
     * @param str The string to convert into an object.
     *
     * @return The object created.
     */
    static Building deserialize(std::string str);
    
private:
    //Private properties
    const std::string buildingButtonText;
    const std::string buildingAction;
    const unsigned int buildingMaxHealth;
    const unsigned int buildingController;
    
    //Location
    const unsigned int buildingX;
    const unsigned int buildingY;
    
    unsigned int buildingHealth;
};

#endif /* Building_hpp */
