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

class Creature {
public:
    //Constructors
    Creature(Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, unsigned int speed, unsigned int vision, unsigned int range, unsigned int cost, Direction startDirection);
    
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
    const unsigned int cost() { return this->creatureCost; }

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
    const unsigned int creatureCost;
    
    unsigned int creatureHealth;
    unsigned int creatureEnergy;
    Direction creatureDirection;
    //vector<int> promotions; //Need to import vector, but not now, implement later.
    
    
    //Private member functions
};

#endif /* Creature_hpp */