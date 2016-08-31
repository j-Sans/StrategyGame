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

float Board::getTerrainMovementCostALPHA(Tile origin, Tile destination) {
    
    if (destination.terrain() == OPEN_TERRAIN) {
        return 1.0; //no creature currently requires more or less than one movement point
    } else if (destination.terrain() == MOUNTAIN_TERRAIN) {
        //if (origin.creature()->race() != Dwarf) {
        return 999.0;
        //}else
        return 2.0;
    } else if (destination.terrain() == WATER_TERRAIN) {
        /*if (origin.creature().promotions does not contain amphibious) {
         return 999;
         }
         if (origin.creature().characteristics does not contain flying) {
         return 999;
         }
         */
        
        //promotions and characteristics have not yet been implemented
    } else if (destination.terrain() == FOREST_TERRAIN) {
        //if (origin.creature()->race() == Elf /* || origin.creature()->characteristics contains terrain ignoring, perhaps in array of bools?*/) {
        //  return 1;
        //}
        return 2.0;
    } else if (destination.terrain() == HILL_TERRAIN) {
        //if (/* || origin.creature()->characteristics contains terrain ignoring, perhaps in array of bools?*/) {
        return 1.0;
        //} else
        return 2.0; //no creature currently requires more or less than two movement points
    } else if (destination.terrain() == SWAMP_TERRAIN) {
        //if (|| origin.creature()->characteristics contains terrain ignoring, perhaps in array of bools?) {
        return 1.0;
        //} else
        return 2.0; //no creature currently requires more or less than two movement points
    } else if (destination.terrain() == ROAD_TERRAIN) {
        return 0.5;
    }
    
    return 1.0;
}


/*
 Terrain Attack Pathing
 LIGHT RANGED:
 If on hill, can shoot into forest and jungle.
 If on plains or water, can only shoot into first layer of forest and jungle
 If on mountain, can shoot into forest and jungle with +1 range but -25% combat strength.
 If in forest and jungle, can only shoot into first layer of forest and jungle
 
 HEAVY RANGED:
 Same as Light Ranged except can shooting through forest and jungle takes less.
 
 LIGHT MELEE vs. GREAT MELEE
 Same.
 
 TERRAIN IGNORING
 Ignores terrain - like telepathic spells and stuff
 
 
 
 Terrain Attack Modifiers:
 
 LIGHT RANGED and HEAVY RANGED:
 No difference in attack damage.
 
 LIGHT MELEE vs. GREAT MELEE
 GreatMelee has combat bonus on hills
 
 TERRAIN IGNORING
 No combat modifier
 
 General Combat Modifiers:
 Half of missing hp % is deducted as a combat debuff.
 Flanking Bonus grants 10% combat bonus per flanking unit.
 */



//The cost is in range, it deducts cost from range.
float Board::getTerrainAttackCost (Tile origin, Tile destination) {
    
    if (destination.terrain() == OPEN_TERRAIN) {
        
        return 1;
        
    } else if (destination.terrain() == MOUNTAIN_TERRAIN) {
        
        return 999;
        
    } else if (destination.terrain() == WATER_TERRAIN) {
        
        return 1;
        
    } else if (destination.terrain() == FOREST_TERRAIN) {
        
        return 999; //need to figure out how to make this return (remaining energy when it hits the forest - 1)
        
    } else if (destination.terrain() == HILL_TERRAIN) {
        
        return 2;
        
    } else if (destination.terrain() == SWAMP_TERRAIN) {
        
    } else if (destination.terrain() == ROAD_TERRAIN) {
        
    }
    
    return 1;
}

//Public member functions
bool Board::moveCreatureByDirection(unsigned int x, unsigned int y, unsigned int direction) {
    
    /* Note:
     * The following sections contain for loop that iterates through the list of board tiles with creatures until it finds the designated one, and it deletes it.
     * It is known that this is incredibly inefficient.
     * However, currently there are not too many board spaces that will have creatures for testing purposes.
     
     Ways to fix this in the future:
     * Make a sort function to get the list sorted in a way that is easy to go through to find the right spot.
     * Use std::optional<Creature> instead of Creature* in Tile.hpp
     std::optional (or std::experimental::optional) may not have come out yet, which is a slight problem.
     There were issues getting it to work on 2013 MacBook Pro in Xcode 7, without much trying
     * Download and add the boost library optional functionality
     */
    
    
    if (x >= this->gameBoard.size())
        throw std::range_error("X out of range");
    if (y >= this->gameBoard[0].size())
        throw std::range_error("Y out of range");
    
    if (direction == NORTH) {
        if (y > 0 && !this->gameBoard[x][y - 1].occupied()) {
            //Add the creature to the new tile
            this->gameBoard[x][y - 1].setCreature(this->gameBoard[x][y].creature());
            
            //Remove the creature from the old tile
            this->gameBoard[x][y].setCreature(nullptr);
            
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Internal: " << x << ", " << y << ' ' << "relocated North to " << x << ", " << y - 1 << '\n';
#endif
            
            //Decrement the creature's energy by the terrain cost
            this->gameBoard[x][y - 1].creature()->decrementEnergy(getTerrainMovementCostALPHA(this->gameBoard[x][y], this->gameBoard[x][y - 1]));
            
            //Find the creature, and update its location on the board
            for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
                if (listIter->x() == x && listIter->y() == y) {
                    listIter->move(NORTH); //Moves the creature north a tile. y can't be 0 because that is checked above
                    break;
                }
            }
            
        } else { //Can't move to the tile
            return false;
        }
    } else if (direction == EAST) {
        if (x > 0 && !this->gameBoard[x - 1][y].occupied()) {
            //Add the creature to the new tile
            this->gameBoard[x - 1][y].setCreature(this->gameBoard[x][y].creature());
            
            //Remove the creature from the old tile
            this->gameBoard[x][y].setCreature(nullptr);
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Internal: " << x << ", " << y << ' ' << "relocated East to " << x - 1 << ", " << y << '\n';
#endif
            
            //Decrement the creature's energy by the terrain cost
            this->gameBoard[x - 1][y].creature()->decrementEnergy(getTerrainMovementCostALPHA(this->gameBoard[x][y], this->gameBoard[x - 1][y]));
            
            //Find the creature, and update its location on the board
            for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
                if (listIter->x() == x && listIter->y() == y) {
                    listIter->move(EAST); //Moves the creature west a tile. x can't be 0 because that is checked above
                    break;
                }
            }
            
        } else {
            return false;
        }
    } else if (direction == SOUTH) {
        if (y < this->gameBoard[x].size() - 1 && !this->gameBoard[x][y + 1].occupied()) {
            //Add the creature to the new tile
            this->gameBoard[x][y + 1].setCreature(this->gameBoard[x][y].creature());
            
            //Remove the creature from the old tile
            this->gameBoard[x][y].setCreature(nullptr);
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Internal: " << x << ", " << y << ' ' << "relocated South to " << x << ", " << y + 1 << '\n';
#endif
            
            //Decrement the creature's energy by the terrain cost
            this->gameBoard[x][y + 1].creature()->decrementEnergy(getTerrainMovementCostALPHA(this->gameBoard[x][y], this->gameBoard[x][y + 1]));
            
            //Find the creature, and update its location on the board
            for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
                if (listIter->x() == x && listIter->y() == y) {
                    listIter->move(SOUTH); //Moves the creature south a tile. y can't be the maximum because that is checked above
                    break;
                }
            }
            
        } else {
            return false;
        }
    } else if (direction == WEST) {
        if (x < this->gameBoard.size() - 1 && !this->gameBoard[x + 1][y].occupied()) {
            //Add the creature to the new tile
            this->gameBoard[x + 1][y].setCreature(this->gameBoard[x][y].creature());
            
            //Remove the creature from the old tile
            this->gameBoard[x][y].setCreature(nullptr);
#ifdef MOVEMENT_CONSOLE_OUTPUT
            std::cout << "Internal: " << x << ", " << y << ' ' << "relocated West to " << x + 1 << ", " << y << '\n';
#endif
            
            //Decrement the creature's energy by the terrain cost
            this->gameBoard[x + 1][y].creature()->decrementEnergy(getTerrainMovementCostALPHA(this->gameBoard[x][y], this->gameBoard[x + 1][y]));
            
            //Find the creature, and update its location on the board
            for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
                if (listIter->x() == x && listIter->y() == y) {
                    listIter->move(WEST); //Moves the creature south a tile. x can't be the maximum because that is checked above
                    break;
                }
            }
            
        } else {
            return false;
        }
    }
    return true;
}

bool Board::moveCreatureByLocation(unsigned int x, unsigned int y, unsigned int destinationX, unsigned int destinationY) {
    if (x >= this->gameBoard.size()) //is this protection really necessary?
        throw std::range_error("Initial x out of range");
    if (y >= this->gameBoard[x].size())
        throw std::range_error("Initial y out of range");
    if (destinationX >= this->gameBoard.size())
        throw std::range_error("Destination x out of range");
    if (destinationY >= this->gameBoard[destinationX].size())
        throw std::range_error("Destination y out of range");
    
    Creature* creature = this->gameBoard[x][y].creature();
    
    if (creature == nullptr) //If there is no creature at the specified location, no moving happens
        return false;
    
    if (destinationX == x && destinationY == y) //If the creature is at the destination, no moving happens
        return false;
    
    if (this->gameBoard[destinationX][destinationY].occupied()) //If the destination is occupied, no moving happens
        return false;
    
    unsigned int distance = this->tileDistances(x, y, destinationX, destinationY);
    
    if (this->tileDistances(x, y, destinationX, destinationY) > creature->energy()) //If the creature doesn't have enough energy to travel the indicated distance, no moving happens
        return false;
    
    //If none of the above errors stop the creature from moving, move the creature
    
    //Add the creature to the new tile
    this->gameBoard[destinationX][destinationY].setCreature(creature);
    
    //Remove the creature from the old tile
    this->gameBoard[x][y].setCreature(nullptr);
    
    //Decrement the creature's energy by 1
    creature->decrementEnergy(distance);
    
    //Find the creature, and update its location on the board
    for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
        if (listIter->x() == x && listIter->y() == y) {
            //Moves the creature in the list to that spot
            listIter->setLocation(destinationX, destinationY);
            break;
        }
    }
    
    return true;
}

/*
 * TO ADD:
 * MODIFIER VALUE
 */
bool Board::attack(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY, int* attackDamage, int* defendDamage) {
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
    Tile* defender = &this->gameBoard[defenderX][defenderY];
    
    if (defender->creature() != nullptr) {
        //If both creatures are melee creatures
        if (attacker->creature()->melee()) {
            
            //Check to make sure the creatures are one tile away from each other
            unsigned int distanceBetweenTiles;
            distanceBetweenTiles = tileDistances(attackerX, attackerY, defenderX, defenderY); //An error is only thrown if arguments are out of range, but that is checked above
            
            if (distanceBetweenTiles > this->gameBoard[attackerX][attackerY].creature()->range()) {
                return false; //No combat occurs
            } else {
                
                int damageDealtByAttacker = attacker->creature()->attack();
                
                bool defenderDied = defender->creature()->takeDamage(damageDealtByAttacker);
                attacker->creature()->useAllEnergy();
                
                if (attackDamage != nullptr)
                    *attackDamage = damageDealtByAttacker;
                
                //If the defender is a melee fighter and survived, it can strike back
                if (!defenderDied && defender->creature()->melee()) {
                    int damageDealtByDefender = defender->creature()->attack();
                    
                    bool attackerDied = attacker->creature()->takeDamage(damageDealtByDefender);
                    
                    if (defendDamage != nullptr)
                        *defendDamage = damageDealtByDefender;
                    
                    if (attackerDied) {
                        this->deleteCreature(attacker->x(), attacker->y());//Remove the dead creature
                    }
                } else if (defenderDied) {
                    this->deleteCreature(defender->x(), defender->y()); //Remove the dead creature
                }
                
                return true; //Combat occurs
            }
            
        } else { //The attacker is a range fighter so there can be no strike back. To consider: other range units can strike back?
            int damageDealtByAttacker = attacker->creature()->attack();
            
            bool defenderDied = defender->creature()->takeDamage(damageDealtByAttacker);
            attacker->creature()->useAllEnergy();
            
            if (attackDamage != nullptr)
                *attackDamage = damageDealtByAttacker;
            
            if (defenderDied) {
                this->deleteCreature(defender->x(), defender->y()); //Remove the dead creature
            }
            
            return true;
        }
    } else if (defender->building() != nullptr) {
        //If both creatures are melee creatures
        if (attacker->creature()->melee()) {
            
            //Check to make sure the creatures are one tile away from each other
            unsigned int distanceBetweenTiles;
            distanceBetweenTiles = tileDistances(attackerX, attackerY, defenderX, defenderY); //An error is only thrown if arguments are out of range, but that is checked above
            
            if (distanceBetweenTiles > this->gameBoard[attackerX][attackerY].creature()->range()) {
                return false; //No combat occurs
            } else {
                
                int damageDealtByAttacker = attacker->creature()->attack();
                
                bool defenderDied = defender->building()->takeDamage(damageDealtByAttacker);
                attacker->creature()->useAllEnergy();
                
                if (attackDamage != nullptr)
                    *attackDamage = damageDealtByAttacker;
                
                if (defenderDied) {
                    this->deleteBuilding(defender->x(), defender->y()); //Remove the dead creature
                }
                
                return true; //Combat occurs
            }
            
        } else { //The attacker is a range fighter so there can be no strike back. To consider: other range units can strike back?
            int damageDealtByAttacker = attacker->creature()->attack();
            
            bool defenderDied = defender->building()->takeDamage(damageDealtByAttacker);
            attacker->creature()->useAllEnergy();
            
            if (attackDamage != nullptr)
                *attackDamage = damageDealtByAttacker;
            
            if (defenderDied) {
                this->deleteBuilding(defender->x(), defender->y()); //Remove the dead creature
            }
            
            return true;
        }
    } else
        return false; //No building or creature to attack
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
    
    this->creatures.push_back(creature);
    
    this->gameBoard[x][y].setCreature(&this->creatures.back());
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
     * Make a sort function to get the list sorted in a way that is easy to go through to find the right spot.
     * Use std::optional<Creature> instead of Creature* in Tile.hpp
        std::optional (or std::experimental::optional) may not have come out yet, which is a slight problem.
        There were issues getting it to work on 2013 MacBook Pro in Xcode 7, without much trying
     * Download and add the boost library optional functionality
     */
    
    for (auto listIter = this->creatures.begin(); listIter != this->creatures.end(); listIter++) {
        if (listIter->x() == x && listIter->y() == y) {
            this->gameBoard[x][y].setCreature(nullptr);
            this->creatures.erase(listIter); //Delete the creature from the list if it is the specified creature.
            break;
        }
    }
    
    //If no creature is deleted in the loop, then there was no creature at that point, which is also fine.
}

void Board::setBuilding(unsigned int x, unsigned int y, Building building) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    this->buildings.push_back(building);
    
    this->gameBoard[x][y].setBuilding(&this->buildings.back());
}

void Board::deleteBuilding(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    /* Note:
     * The following for loop iterates through the list of board tiles with buildings until it finds the designated one, and it deletes it.
     * It is known that this is incredibly inefficient.
     * However, currently there are not too many board spaces that will have buildings for testing purposes.
     
     Ways to fix this in the future:
     * Make a sort function to get the list sorted in a way that is easy to go through to find the right spot.
     * Use std::optional<Building> instead of Building* in Tile.hpp
     std::optional (or std::experimental::optional) may not have come out yet, which is a slight problem.
     There were issues getting it to work on 2013 MacBook Pro in Xcode 7, without much trying
     * Download and add the boost library optional functionality
     */
    
    for (auto listIter = this->buildings.begin(); listIter != this->buildings.end(); listIter++) {
        if (listIter->x() == x && listIter->y() == y) {
            this->gameBoard[x][y].setBuilding(nullptr);
            this->buildings.erase(listIter); //Delete the building from the list if it is the specified creature.
            break;
        }
    }
    
    //If no building is deleted in the loop, then there was no creature at that location, which is also fine.
}

void Board::setStyle(unsigned int x, unsigned int y, Style style) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    this->gameBoard[x][y].setStyle(style);
}

bool Board::setDirection(unsigned int x, unsigned int y, unsigned int direction) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    if (this->gameBoard[x][y].creature() == nullptr)
        return false;
    
    if (direction > 3)
        return false;
    
    this->gameBoard[x][y].setDirection(direction);
    return true;
}

void Board::setDamage(unsigned int x, unsigned int y, unsigned int damage, float time) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    this->gameBoard[x][y].setDamage(damage, time);
}

Tile Board::get(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range");
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range");
    }
    
    return this->gameBoard[x][y];
}

unsigned int Board::width() {
    return (unsigned int)this->gameBoard.size();
}

unsigned int Board::height(unsigned int x) {
    return (unsigned int)this->gameBoard[x].size();
}