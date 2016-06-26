//
//  Board.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Board.hpp"

void Board::moveCreature(unsigned int x, unsigned int y, Direction moveTo) {

    if (x >= this->gameBoard.size()) //is this protection really necessary?
        throw std::range_error("X out of range");
    if (y >= this->gameBoard[0].size())
        throw std::range_error("Y out of range");
    
    if (moveTo == North) { //graphics and board aren't linked yet, so i'll temporarily set y to north and x to east. I think this should make north the top-left corner of screen?
        gameBoard[x][y+1].setCreature(gameBoard[x][y].creature());
        gameBoard[x]y].setCreature(nullptr)
    } else if (moveTo == East) {
        gameBoard[x+1][y].setCreature(gameBoard[x][y].creature());
        gameBoard[x]y].setCreature(nullptr)
       
    } else if (moveTo == South) {
        gameBoard[x][y-1].setCreature(gameBoard[x][y].creature());
        gameBoard[x]y].setCreature(nullptr)
        
    } else if (moveTo == West) {
        gameBoard[x-1][y].setCreature(gameBoard[x][y].creature());
        gameBoard[x]y].setCreature(nullptr)
        
    }
}