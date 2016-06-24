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
    Forest,
    Mountain,
    //More to be added later
};

class Tile {
public:
    //Constructors
    
    //Destructor
    ~Tile();
    
    //Public properties
    void setCreature(Creature *creature);
    
    //Public member functions
    
    //Get methods
    const Creature* creature() { return this->tileCreature; }
    const Terrain terrain() { return this->tileTerrain; }
    const bool occupied() { return this->tileCreature != nullptr ? true : false; }
    
    //I think we should impliment these later because they may be complex to program without that much reward
    //const bool vision() { return this->blocksVision; }
    //const bool rough() { return this->isRoughTerrain; }
    
private:
    //Private properties
    Creature* tileCreature = nullptr;
    Terrain tileTerrain = Open;
    
    //const bool blocksVision;
    //const bool isRoughTerrain; //slows movement
    
    //Private member functions
};

#endif /* Tile_hpp */
