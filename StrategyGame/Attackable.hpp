//
//  Attackable.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#ifndef Attackable_hpp
#define Attackable_hpp

#include <string>

class Attackable {
public:
    
    //Public member functions
    
    /*!
     * @return The maximum health of this object.
     */
    unsigned int maxHealth();
    
    /*!
     * @return The current health of this object.
     */
    unsigned int health();
    
    /*!
     * @return The x location of this object on the board.
     */
    unsigned int x();
    
    /*!
     * @return The y location of this object on the board.
     */
    unsigned int y();
    
    /*!
     * @return An unsigned int representing the controller of the object.
     */
    unsigned int controller();
    
protected:
    //Private properties
    
    unsigned int maxHealthVal;
    unsigned int controllerVal;
    
    //Location
    unsigned int xVal;
    unsigned int yVal;
    
    unsigned int buildingHealth;
};

#endif /* Attackable_hpp */
