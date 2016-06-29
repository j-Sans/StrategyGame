//
//  Board.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Board.hpp"

//Constructors

Board::Board(std::vector<std::vector<Tile> > board) : gameBoard(board) {
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

/*
 * TO ADD:
 * MODIFIER VALUE
 */
bool Board::attack(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY) {
    if (attackerX >= this->gameBoard.size()) {
        throw std::range_error("Attacker x out of range");
    }
    if (attackerY >= this->gameBoard[attackerX].size()) {
        throw std::range_error("Attacker y out of range");
    }
    if (defenderX >= this->gameBoard.size()) {
        throw std::range_error("Defender x out of range");
    }
    if (defenderY >= this->gameBoard[defenderX].size()) {
        throw std::range_error("Defender y out of range");
    }
    
    //Using pointers to get tiles by reference
    Tile* attacker = &this->gameBoard[attackerX][attackerY];
    Tile* defender = &this->gameBoard[attackerX][attackerY];
    
    //If both creatures are melee creatures
    if (attacker->creature()->melee()) {
        
        //Check to make sure the creatures are one tile away from each other
        unsigned int distanceBetweenTiles;
        try {
            distanceBetweenTiles = tileDistances(attackerX, attackerY, defenderX, defenderY);
        } catch (std::exception) {
            return false; //Maybe it should throw an error instead? This happens only if the arguments are out of range of the board, but that should be caught above
            
            //throw std::range_error("Argument out of range");
        }
        
        if (distanceBetweenTiles > 1) {
            return false; //No combat occurs
        } else {
            bool defenderDied = defender->creature()->takeDamage(attacker->creature()->attack());
            attacker->creature()->useAllEnergy();
            
            //If the defender is a melee fighter and survived, it can strike back
            if (!defenderDied && defender->creature()->melee()) {
                bool attackerDied = attacker->creature()->takeDamage(defender->creature()->attack());
                if (attackerDied) {
                    attacker->setCreature(nullptr); //Remove the dead creature
                }
            } else if (defenderDied) {
                defender->setCreature(nullptr); //Remove the dead creature
            }
            
            return true; //Combat occurs
        }
        
    } else { //The attacker is a range fighter so there can be no strike back. To consider: other range units can strike back?
        bool defenderDied = defender->creature()->takeDamage(attacker->creature()->attack());
        attacker->creature()->useAllEnergy();
        
        if (defenderDied) {
            defender->setCreature(nullptr); //Remove the dead creature
        }
        
        return true;
    }
}

unsigned int Board::tileDistances(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    if (x1 >= this->gameBoard.size()) {
        throw std::range_error("X1 out of range");
    }
    if (y1 >= this->gameBoard[x1].size()) {
        throw std::range_error("Y1 out of range");
    }
    if (x2 >= this->gameBoard.size()) {
        throw std::range_error("X2 out of range");
    }
    if (y2 >= this->gameBoard[x2].size()) {
        throw std::range_error("Y2 out of range");
    }
    
    //math.h 's abs function wasn't working properly
    
    //Get the difference in x coordinates
    int xDisplacement = (int)x1 - (int)x2 < 0 ? x2 - x1 : x1 - x2;
    
    //Get the difference in y coordinates
    int yDisplacement = (int)y1 - (int)y2 < 0 ? y2 - y1 : y1 - y2;
    
    return xDisplacement + yDisplacement;
}

void Board::setCreature(unsigned int x, unsigned int y, Creature creature) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    this->creatures.push_back(CreatureInList(x, y, creature));
    
    this->gameBoard[x][y].setCreature(&this->creatures.back().creature);
    
    /*... UNFINISHED. TO ADD:
     * TEST GRAPHICS WITH THIS CREATURE FUNCTION
     */
}

void Board::deleteCreature(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    /* Note:
     * The following for loop iterates through the list of board tiles with creatures until it finds the designated one, and it deletes it.
     * It is known that this is incredibly inefficient.
     * However, currently there are not too many board spaces that will have creatures for testing purposes.
     
     Ways to fix this in the future:
     * Make a sort functino to get the list sorted in a way that is easy to go through to find the right spot.
     * Use std::optional<Creature> instead of Creature* in Tile.hpp
        std::optional (or std::experimental::optional) may not have come out yet, which is a slight problem.
        There were issues getting it to work on 2013 MacBook Pro in Xcode 7, without much trying
     * Download and add the boost library optional functionality
     */
    
    for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
        if (listIter->x == x && listIter->y == y) {
            this->gameBoard[x][y].setCreature(nullptr);
            this->creatures.erase(listIter); //Delete the creature from the list if it is the specified creature.
            break;
        }
    }
    
    //If no creature is deleted in the loop, then there was no creature at that point, which is also fine.
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