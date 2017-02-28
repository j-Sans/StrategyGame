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

//Standard library includes
#include <string>

//Local includes
#include "Attackable.hpp"

class Building : public Attackable {
public:
    //Constructor
    
    Building(unsigned int x, unsigned int y, unsigned int maxHealth, unsigned int controller);
    
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
     * Serialize this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    std::string serialize() const;
    
    /*!
     * Create an object from a serialized string that can be sent through sockets.
     *
     * @param str The string to convert into an object.
     *
     * @return The object created.
     */
    static Building deserialize(std::string str);
    
private:
};

#endif /* Building_hpp */
