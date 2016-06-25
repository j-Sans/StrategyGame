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

enum Terrain {
    Open,
    Mountain,
    Forest,
    Water,
    CarrotFarm //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.
};

class Tile {
public:
    //Constructors
    Tile(Terrain terrain, unsigned int x, unsigned int y);
    
    //Destructor
    ~Tile();
    
    //Public properties
    void setCreature(Creature *creature);
    
    //Public member functions
    
    //Get methods
    const unsigned int x() { return this->tileX; }
    const unsigned int y() { return this->tileY; }
    const Creature* creature() { return this->tileCreature; }
    const Terrain terrain() { return this->tileTerrain; }
    const bool occupied() { return this->tileCreature != nullptr ? true : false; }
    
    //I think we should impliment these later because they may be complex to program without that much reward
    //const bool vision() { return this->blocksVision; }
    //const bool rough() { return this->isRoughTerrain; }
    
private:
    //Private properties
    const unsigned int tileX;
    const unsigned int tileY;
    Creature* tileCreature = nullptr;
    Terrain tileTerrain = Open;
    
    //const bool blocksVision;
    //const bool isRoughTerrain; //slows movement
    
    //Private member functions
};

#endif /* Tile_hpp */
