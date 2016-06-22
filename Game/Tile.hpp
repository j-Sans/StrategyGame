//
//  Tile.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include "Creature.hpp"

class Tile {
public:
    //Constructors
    Tile(const bool isObstacle);
    
    //Destructor
    ~Tile();
    
    //Public properties
    void setCreature(Creature *creature);
    
    //Public member functions
    
    //Get methods
    const bool passable() { return this->isPassable; }
    
private:
    //Private properties
    Creature* tileCreature = nullptr;
    bool isPassable = true;
    const bool obstacle;
    
    //Private member functions
};

#endif /* Tile_hpp */
