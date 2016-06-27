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
 */
class Board {
public:
    //Constructor
    
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
    
private:
    //Private properties
    std::vector<std::vector<Tile> > gameBoard;
    
    //Private member functions
};

#endif /* Board_hpp */
