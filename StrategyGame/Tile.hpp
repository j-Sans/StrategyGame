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

/**
 * A class representing a single tile on the board and, if present, the creature on that tile.
 *
 * @param terrain The type of terrain represented by this tile.
 * @param x The x coordinate of this tile on the board.
 * @param y The y coordinate of this tile on the board.
 */
class Tile {
public:
    //Constructors
    Tile(Terrain terrain, unsigned int x, unsigned int y);
    
    //Destructor
    ~Tile();
    
    //Public properties
    
    //Public member functions
    
    /** 
     * Sets a creature as the creature located in this tile. Deletes the old creature to avoid memory leaks, and adjusts whether the tile is occupied.
     *
     * @param creature A pointer to the new creature. If nullptr, then the spot becomes empty.
     */
    void setCreature(Creature *creature);
    
    //Get methods
    
    /**
     * @return The x position of the tile in the board.
     */
    const unsigned int x();
    
    /**
     * @return The y position of the tile in the board.
     */
    const unsigned int y();
    
    /**
     * @return A pointer to the creature at this tile.
     */
    Creature* creature();
    
    /**
     * @return The terrain type of this tile.
     */
    Terrain terrain();
    
    /**
     * @return A boolean representing if there is a creature on this tile currently.
     */
    bool occupied();
    
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
