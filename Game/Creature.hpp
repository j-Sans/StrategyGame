//
//  Creature.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Creature_hpp
#define Creature_hpp

enum Race {
    Human,
    Elf,
    Dwarf,
    Orc,
    Goblin,
    Undead,
    Vampire,
};

enum Direction {
    North,
    East,
    South,
    West,
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
    Creature(Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, unsigned int speed, unsigned int vision, unsigned int range, Direction startDirection);
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    bool takeDamage(unsigned int damage);
    
    //Get methods
    const Race race() { return this->creatureRace; }
    const unsigned int maxHealth() { return this->creatureMaxHealth; }
    const unsigned int maxEnergy() { return this->creatureMaxEnergy; }
    const unsigned int attack() { return this->creatureAttack; }
    const unsigned int speed() { return this->creatureSpeed; }
    const unsigned int vision() { return this->creatureVision; }
    const unsigned int range() { return this->creatureRange; }

    const unsigned int health() { return this->creatureHealth; }
    const unsigned int energy() { return this->creatureEnergy; }
    const Direction direction() { return this->creatureDirection; }
    
private:
    //Private properties
    const Race creatureRace;
    const unsigned int creatureMaxHealth;
    const unsigned int creatureMaxEnergy;
    const unsigned int creatureAttack;
    const unsigned int creatureSpeed;
    const unsigned int creatureVision;
    const unsigned int creatureRange;
    
    unsigned int creatureHealth;
    unsigned int creatureEnergy;
    Direction creatureDirection;
    //vector<int> promotions; //Need to import vector, but not now, implement later.
    
    
    //Private member functions
};

#endif /* Creature_hpp */