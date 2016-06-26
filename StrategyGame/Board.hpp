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

class Board {
public:
    //Constructor
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    void moveCreature(unsigned int x, unsigned int y, Direction moveTo);
    
    //Public get functions
    
private:
    //Private properties
    std::vector<std::vector<Tile> > gameBoard;
    
    //Private member functions
};

#endif /* Board_hpp */
