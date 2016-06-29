//
//  Board.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <math.h>
#include <vector>
#include <list>
#include <exception>

#include "Tile.hpp"

/**
 * A board class representing a 2D vector of the board.
 *
 * @param board A 2D vector of Tiles representing the game board.
 */
class Board {
public:
    //Constructor
    
    //Default constructor. Don't use this, it is only to allow shader objects to exist in classes without being declared first.
//    Board();
    
    Board(std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    
    /**
     * Move a creature from the designated spot in the designated direction.
     * Possible errors include if the destination is occupied or if the destination is off of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param moveTo The direction in which to move. Possible direction types include North, East, South, and West.
     */
    void moveCreature(unsigned int x, unsigned int y, Direction moveTo);
    
    /**
     * Have one creature attack the other creature.
     * Possible errors include if the indices are off of the board.
     *
     * @param attackerX The x index of the attacker in the board.
     * @param attackerY The y index of the attacker in the board.
     * @param defenderX The x index of the defender in the board.
     * @param defenderY The y index of the ∂efender in the board.
     *
     * @return Whether combat occurred or not, for various reasons.
     */
    bool attack(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY);
    
    /**
     * Get the distance (in taxicab geometry) from one tile to another.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x1 The x index of the first coordinate in the board.
     * @param y1 The y index of the first coordinate in the board.
     * @param x2 The x index of the second coordinate in the board.
     * @param y2 The y index of the second coordinate in the board.
     *
     * @return The distance (in taxicab geometry) from one tile to another.
     */
    unsigned int tileDistances(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    
    /**
     * Set a creature in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param creature A creature object to be set in the board at the designated spot.
     */
    void setCreature(unsigned int x, unsigned int y, Creature creature);
    
    //Public get functions
    
    /**
     * Get the tile at the spot (x,y) from the board.
     * Possible errors include if the x or y coordinates are greater than or equal to the size of the respective vectors.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     *
     * @return The tile object at this location in the board.
     */
    Tile get(unsigned int x, unsigned int y);
    
    /**
     * @return The x-side width of the board.
     */
    unsigned int width();
    
    /**
     * @param x The column x coordinate from which to get the size.
     *
     * @return The y-side height of the board.
     */
    unsigned int height(unsigned int x);
    
private:
    //Private properties
    std::vector<std::vector<Tile> > gameBoard;
    std::list<Creature> creatures; //List of creatures on the game board, for board tiles to have pointers to
    
    //Private member functions
};

#endif /* Board_hpp */
