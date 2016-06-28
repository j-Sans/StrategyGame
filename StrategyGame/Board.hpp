//
//  Board.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <vector>
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
    
    //Private member functions
};

#endif /* Board_hpp */
