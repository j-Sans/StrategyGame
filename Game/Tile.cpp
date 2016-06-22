//
//  Tile.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Tile.hpp"

//Constructors
/** Creates a tile object
 *
 * @param isObstacle A boolean representing if the tile is an impassable obstacle
 */
Tile::Tile(bool isObstacle) : obstacle(isObstacle) {
    if (isObstacle) {
        this->isPassable = false;
    }
}

//Destructor

/** Deletes this tile.
 */
Tile::~Tile() {
    delete this->tileCreature;
}

//Public member functions
/** Sets a creature as the creature located in this tile. Deletes the old creature to avoid memory leaks.
 *
 * @param creature A pointer to the new creature.
 */
void Tile::setCreature(Creature *creature) {
    if (this->tileCreature != nullptr)
        delete this->tileCreature; //Prevent memory leaks
    
    this->tileCreature = creature; //Set the creature at this tile as the inputted creature
    
    //Make the tile impassible as long as there is a creature, and passable otherwise, if it's not an obstacle
    if (this->tileCreature != nullptr)
        this->isPassable = false;
    else if (!this->obstacle)
        this->isPassable = true;
}