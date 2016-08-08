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

class Building {
public:
    //Constructor
    Building(unsigned int x, unsigned int y, unsigned int maxHealth, unsigned int cost, unsigned int controller);
    
    //Destructor
    
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
     * @return The maximum health of this building.
     */
    const unsigned int maxHealth();
    
    /*!
     * @return The current health of this building.
     */
    unsigned int health();
    
    /*!
     * @return The cost of this building
     */
    const unsigned int cost();
    
    /*!
     * @return The x location of this creature on the board.
     */
    const unsigned int x();
    
    /*!
     * @return The y location of this creature on the board.
     */
    const unsigned int y();
    
    /*!
     * @return An unsigned int representing the controller of the building.
     */
    const unsigned int controller();
    
private:
    //Private properties
    const unsigned int buildingMaxHealth;
    const unsigned int buildingCost;
    const unsigned int buildingController;
    
    //Location
    const unsigned int buildingX;
    const unsigned int buildingY;
    
    unsigned int buildingHealth;
};

#endif /* Building_hpp */
