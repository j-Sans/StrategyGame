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
    const bool obstacle() { return this->isObstacle; }
    const bool vision() { return this->blocksVision; }
    const bool rough() { return this->isRoughTerrain; }
    const bool mountain() { return this-> mountain; }
    
private:
    //Private properties
    Creature* tileCreature = nullptr;
    bool isPassable = true;
    const bool isObstacle;
    const bool blocksVision;
    const bool isRoughTerrain; //slows movement
    const bool mountain;
    //Private member functions
};

#endif /* Tile_hpp */
