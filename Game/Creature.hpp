//
//  Creature.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Creature_hpp
#define Creature_hpp

class Creature {
public:
    //Constructors
    Creature(unsigned int maxHealth);
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    bool takeDamage(unsigned int damage);
    
    //Get methods
    const unsigned int maxHealth() { return this->creatureMaxHealth; }
    const unsigned int health() { return this->creatureHealth; }
    
private:
    //Private properties
    const unsigned int creatureMaxHealth;
    unsigned int creatureHealth;
    
    //Private member functions
};

#endif /* Creature_hpp */