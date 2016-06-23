//
//  Creature.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Creature_hpp
#define Creature_hpp

enum race {
    human,
    elf,
    dwarf,
    orc,
    goblin,
    undead,
    vampires
};

enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

/*
 enum promotions {
    //Tier 1 (also i know this isnt how u format enums but this is just a plan list thing. Choose 1 per promo.
        Forest 1, Hill 1, Open 1.
    //Tier 2
        Forest 2, Hill 2, Open 2, Cover 1, Flank 1, Medic 1, Anti-Cav 1.
    //Tier 3
        Forest 3, Hill 3, Open 3, Cover 2, Flank 2, Medic 2, Vision 1, Fast 1.
    //Tier 4
        Double Attack, Heal Per Turn, Heal Per Kill, Leadership, Range, Looter, Truesight, Impervious, Toxic.
};
 */

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
    unsigned int maxEnergy() {return this->creatureMaxEnergy; }
    unsigned int attack() {return this->creatureAttack; }
    unsigned int vision() {return this->creatureVision; }
    unsigned int range() {return this->creatureRange; }

    unsigned int health() { return this->creatureHealth; }
    unsigned int energy() {return this->creatureEnergy; }
    unsigned int direction() {return this->creatureDirection; }
    
private:
    //Private properties
    const unsigned int creatureMaxHealth;
    unsigned int creatureMaxEnergy;
    unsigned int creatureAttack;
    unsigned int creatureVision;
    unsigned int creatureRange;
    
    unsigned int creatureHealth;
    unsigned int creatureEnergy;
    unsigned int creatureDirection;
    //vector<int> promotions; //Need to import vector, but not now, implement later.
    
    
    //Private member functions
};

#endif /* Creature_hpp */