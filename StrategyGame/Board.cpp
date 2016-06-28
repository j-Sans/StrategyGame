//
//  Board.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Board.hpp"

//Constructors

//Only so that Game.hpp can have a board property without declaring it initially. No other purpose.
//Board::Board() {}

Board::Board(std::vector<std::vector<Tile> > board) : gameBoard(board) {
//    this->gameBoard = board;
}

//Public member functions
void Board::moveCreature(unsigned int x, unsigned int y, Direction moveTo) {
    if (x >= this->gameBoard.size()) //is this protection really necessary?
        throw std::range_error("X out of range");
    if (y >= this->gameBoard[0].size())
        throw std::range_error("Y out of range");
    
    if (moveTo == North) {
        if (y > 0 && !this->gameBoard[x][y - 1].occupied()) {
            this->gameBoard[x][y - 1].setCreature(this->gameBoard[x][y].creature());
            this->gameBoard[x][y].setCreature(nullptr);
        } else if (this->gameBoard[x][y - 1].occupied()) {
            throw std::range_error("Northern tile occupied");
        } else {
            throw std::range_error("No northern tile available");
        }
    } else if (moveTo == West) {
        if (x > 0 && !this->gameBoard[x - 1][y].occupied()) {
            this->gameBoard[x - 1][y].setCreature(this->gameBoard[x][y].creature());
            this->gameBoard[x][y].setCreature(nullptr);
        } else if (this->gameBoard[x - 1][y].occupied()) {
            throw std::range_error("Western tile occupied");
        } else {
            throw std::range_error("No western tile available");
        }
    } else if (moveTo == South) {
        if (y < this->gameBoard[x].size() - 1 && !this->gameBoard[x][y + 1].occupied()) {
            this->gameBoard[x][y + 1].setCreature(this->gameBoard[x][y].creature());
            this->gameBoard[x][y].setCreature(nullptr);
        } else if (this->gameBoard[x][y + 1].occupied()) {
            throw std::range_error("Southern tile occupied");
        } else {
            throw std::range_error("No southern tile available");
        }
    } else if (moveTo == East) {
        if (x < this->gameBoard.size() - 1 && !this->gameBoard[x + 1][y].occupied()) {
            this->gameBoard[x + 1][y].setCreature(this->gameBoard[x][y].creature());
            this->gameBoard[x][y].setCreature(nullptr);
        } else if (this->gameBoard[x + 1][y].occupied()) {
            throw std::range_error("Eastern tile occupied");
        } else {
            throw std::range_error("No eastern tile available");
        }
    }
}

void processCreatureCombat(Tile attackSquare, Tile defendSquare) {
    int totalmodifier = 1; //calculateTotalModifier();
    if (defendSquare.creature().hasMeleeAttack() = true && attackSquare.creature().hasMeleeAttack() = true) {
        //The order of this gives attacking an advantage. If the attacker kills the defender, the defender won't be able to strike back.
        if (defendSquare.takeDamage(/*   totalmodifier*     */  attackSquare.creature().attack()) = true) {
            attackSquare.creature().energy() = 0;
        }
        else {
            attackSquare.creature().energy() = 0;
            attackSquare.creature().takeDamage(/*     totalmodifier*   */  defendSquare.creature().attack());
            //tbh this would be easier for me if takeDamage were a void function and not a bool function. will talk to u later about making it a void function.
        }

        
    } else { //Melee -> Melee both take damage. Melee -> Ranged, defender takes damage. Ranged -> Melee/Ranged, defender takes damage.
        defendSquare.creature().takeDamage(attackSquare.creature().attack());
        attackSquare.creature().energy() = 0;
    }
}

Tile Board::get(unsigned int x, unsigned int y) {
    if (x >= gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (x >= gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    return gameBoard[x][y];
}

unsigned int Board::width() {
    return (unsigned int)this->gameBoard.size();
}

unsigned int Board::height(unsigned int x) {
    return (unsigned int)this->gameBoard[x].size();
}