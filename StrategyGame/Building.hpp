//
//  Building.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Building_hpp
#define Building_hpp

class Building {
public:
    //Constructor
    
    //Destructor
    
    //Public member functions
    
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
    
private:
    //Private properties
    const unsigned int buildingMaxHealth;
    const unsigned int buildingCost;
    
    unsigned int buildingHealth;
};

#endif /* Building_hpp */
