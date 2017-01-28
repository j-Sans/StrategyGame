//
//  Board.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Board.hpp"

//Constructors

Board::Board(std::vector<std::vector<Tile> > board) : gameBoard(board) {}

float Board::getTerrainMovementCost(Tile origin, Tile destination) {
    
    /*if (origin.creature().characteristics().find(TerrainIgnoring) != std::string::npos) {
        return 1;
    }*/
    
    if (destination.terrain() == OPEN_TERRAIN) {
        return 1.0; //no creature currently requires more or less than one movement point
    } else if (destination.terrain() == MOUNTAIN_TERRAIN) {
        if (origin.creature()->race() != Dwarf) {
            return 999.0;
        } else return 2.0;
    } else if (destination.terrain() == WATER_TERRAIN) {
        /*if (origin.creature().find(Amphibious) != std::string::npos) {
         return 999;
         }
         if (origin.creature().characteristics().find(Flying) != std::string::npos) {
         return 999;
         }
         */
        
        //promotions and characteristics have not yet been implemented
    } else if (destination.terrain() == FOREST_TERRAIN) {
        if (origin.creature() != nullptr) {
            if (origin.creature()->race() == Elf /* || origin.creature()->characteristics contains terrain ignoring, perhaps in array of bools?*/) {
                return 1;
            }
        }
        return 2.0;
    } else if (destination.terrain() == HILL_TERRAIN) {
        return 2.0; //no creature currently requires more or less than two movement points
        
    } else if (destination.terrain() == SWAMP_TERRAIN) {
        return 3.0; //no creature currently requires more or less than two movement points
        
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
    
    if (origin.terrain() == HILL_TERRAIN && destination.terrain() != MOUNTAIN_TERRAIN) {
        return 1;
    }
    else if (destination.terrain() == MOUNTAIN_TERRAIN) {
        return INT_MAX;
    }
    else if (destination.terrain() == FOREST_TERRAIN || destination.terrain() == HILL_TERRAIN) {
        return 2;
    }
    else return 1;
}

//The cost is in range, it deducts cost from range.
float Board::getTerrainVisionCost (Tile origin, Tile destination) {
    if (origin.terrain() == HILL_TERRAIN && destination.terrain() != MOUNTAIN_TERRAIN) {
        return 1;
    }
    else if (destination.terrain() == MOUNTAIN_TERRAIN) {
        return INT_MAX;
    }
    else if (destination.terrain() == FOREST_TERRAIN || destination.terrain() == HILL_TERRAIN) {
        return 2;
    }
    else return 1;
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
        throw std::range_error("X out of range: " + std::to_string(x));
    if (y >= this->gameBoard[x].size())
        throw std::range_error("Y out of range: " + std::to_string(y));
    
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
            this->gameBoard[x][y - 1].creature()->decrementEnergy(getTerrainMovementCost(this->gameBoard[x][y], this->gameBoard[x][y - 1]));
            
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
            this->gameBoard[x - 1][y].creature()->decrementEnergy(getTerrainMovementCost(this->gameBoard[x][y], this->gameBoard[x - 1][y]));
            
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
            this->gameBoard[x][y + 1].creature()->decrementEnergy(getTerrainMovementCost(this->gameBoard[x][y], this->gameBoard[x][y + 1]));
            
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
            this->gameBoard[x + 1][y].creature()->decrementEnergy(getTerrainMovementCost(this->gameBoard[x][y], this->gameBoard[x + 1][y]));
            
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
 
 Combat Rules:
 Attackers deal damage. Modifiers are applied.
 Defenders deal damage to attacker if attacker is within range. Modifiers are applied.
 
 
 
 Combat Modifiers
 Missing HP:
 Units deal less damage and take more damage equal to half of the percentage of missing health (but not defense)
 
 Flanking:
 Units deal 15% more damage for each flanking creature and deal 15% less damage when being flanked for each flanking creature.
 
 Terrain - based on the terrain of the defender that is being attacked. This is because it is assumed that attackers run into the defending square to attack.
  0% in Open
 -20% in Swamp
 +20% in Forest
 +25% in Hill
 0% in Road
 0% in Water
 +50% in Mountain
 
 Terrain modifiers of the defender are subtracted from the damage of the attacking unit. The defender does not deal extra damage. Swamp causes more damage.
 
 Melee Units can Fortify. If they had full energy on the turn they fortified they heal and gain a +20% Combat Bonus, which becomes 40% on the next turn if the unit remains fortified.
 */

//There needs to be a combat calculator that shows you the attack (after modifiers) and health of attacker and defender when you mouse over the tile of a specific attack.

//Calculate missing HP debuff for combat
float Board::calculateWeaknessDebuff(Tile combatTile) {
    float debuff = -0.5 * (1.0 - (((float)(combatTile.creature()->health())) / (float)(combatTile.creature()->maxHealth())));
#ifdef COMBAT_CONSOLE_OUTPUT
    std::cout << "Injury debuff: " << debuff << '\n';
#endif
    return debuff;
};

//Calculate flanking bonus for melee combat
float Board::calculateFlankingBonus(Tile attacker, Tile defender) {
//    int adjacentEnemies = 0;
    
    //North
    
    //East
    
    //South
    
    //West
    return 0.00;
};

//Calculate terrain modifier for combat
float Board::calculateTerrainModifier(Tile defender) {
    if (defender.terrain() ==  OPEN_TERRAIN) {
        return 0.00;
    }
    if (defender.terrain() ==  FOREST_TERRAIN) {
        return 0.20;
    }
    if (defender.terrain() ==  SWAMP_TERRAIN) {
        return -0.20;
    }
    if (defender.terrain() ==  HILL_TERRAIN) {
        return 0.25;
    }
    if (defender.terrain() ==  ROAD_TERRAIN) {
        return 0.00;
    }
    if (defender.terrain() ==  WATER_TERRAIN) {
        return 0.00;
    }
    if (defender.terrain() ==  MOUNTAIN_TERRAIN) {
        return 0.50;
    }
    return 0.00;
};


bool Board::initiateCombat(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY, int* attackDamage, int* defendDamage) {
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
    
    //Calculate Combat Modifiers
    float attackerCombatModifier = 1.00;
    float defenderCombatModifier = 1.00;
    
    if (defender->creature() != nullptr) {
            
            //Check to make sure the creatures are within range
            unsigned int distanceBetweenTiles;
            distanceBetweenTiles = tileDistances(attackerX, attackerY, defenderX, defenderY); //An error is only thrown if arguments are out of range, but that is checked above
            
            if (distanceBetweenTiles > this->gameBoard[attackerX][attackerY].creature()->range()) {
                return false; //No combat occurs
            } else { //if its not melee its ranged. or terrain ignoring.
                
                //Calculate Attacker Modifiers
                //Missing HP:
                attackerCombatModifier += 0.00+(float)calculateWeaknessDebuff(*attacker);
                
#ifdef COMBAT_CONSOLE_OUTPUT
                std::cout << "Attacker Modifier After Weakness Debuff: " << attackerCombatModifier << '\n';
#endif
                
                //Flanking:        (Only occurs on melee)
                if (attacker->creature()->attackStyle() == Melee) {
                    attackerCombatModifier += calculateFlankingBonus(*attacker, *defender);
                }
                //Terrain:
                attackerCombatModifier -= calculateTerrainModifier(*defender);
#ifdef COMBAT_CONSOLE_OUTPUT
                std::cout << "Attacker Modifier: " << attackerCombatModifier << '\n';
#endif
                int damageDealtByAttacker = round( (float)attacker->creature()->attack() * attackerCombatModifier );
                
                bool defenderDied = defender->creature()->takeDamage(damageDealtByAttacker);
                
                attacker->creature()->useAllEnergy();
                
                if (attackDamage != nullptr)
                    *attackDamage = damageDealtByAttacker;
                
                //If the defender is a melee fighter and survived, it can strike back
                if (!defenderDied && defender->creature()->range() >= distanceBetweenTiles) {
                    
                    //Calculate Defender Modifiers
                    //Missing HP:
                    defenderCombatModifier += (float)calculateWeaknessDebuff(*defender);
                    
#ifdef COMBAT_CONSOLE_OUTPUT
                    std::cout << "Defender Modifier after Weakness Debuff: " << defenderCombatModifier << '\n';
#endif
                    
                    //Flanking:        (Only occurs on melee)
                    if (defender->creature()->attackStyle() == Melee) {
                        defenderCombatModifier += calculateFlankingBonus(*defender, *attacker);
                        //defenderCombatModifier += defender->creature().getFortificationBonus();
                    }

#ifdef COMBAT_CONSOLE_OUTPUT
                    std::cout << "Defender Modifier: " << defenderCombatModifier << '\n';
#endif
                    int damageDealtByDefender = round((float)defender->creature()->attack() * defenderCombatModifier);
                    
                    bool attackerDied = attacker->creature()->takeDamage(damageDealtByDefender);
                    
                    if (defendDamage != nullptr)
                        *defendDamage = damageDealtByDefender;
                    
                    if (attackerDied) {
                        this->deleteCreature(attacker->x(), attacker->y()); //Remove the dead creature
                    }
                    
                    if (defenderDied) {
                        this->deleteCreature(defender->x(), defender->y()); //Remove the dead creature
                    }
                } else if (defenderDied) {
                    this->deleteCreature(defender->x(), defender->y()); //Remove the dead creature
                }
                
                return true; //Combat occurs
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
                    this->deleteBuilding(defender->x(), defender->y()); //Remove the dead building
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
                this->deleteBuilding(defender->x(), defender->y()); //Remove the dead building
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

void Board::resetEnergy(unsigned int player) {
    for (auto creature = this->creatures.begin(); creature != this->creatures.end(); creature++) {
        if (creature->controller() == player) {
            creature->resetEnergy();
        }
    }
}

void Board::setCreature(unsigned int x, unsigned int y, Creature creature) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
    }
    
    this->creatures.push_back(creature);
    
    this->gameBoard[x][y].setCreature(&this->creatures.back());
}

void Board::deleteCreature(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
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
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
    }
    
    this->buildings.push_back(building);
    
    this->gameBoard[x][y].setBuilding(&this->buildings.back());
}

void Board::deleteBuilding(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
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

bool Board::setDirection(unsigned int x, unsigned int y, unsigned int direction) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
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
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
    }
    
    this->gameBoard[x][y].setDamage(damage, time);
}

Tile Board::get(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard.size()) {
        throw std::range_error("X out of range: " + std::to_string(x));
    }
    if (y >= this->gameBoard[x].size()) {
        throw std::range_error("Y out of range: " + std::to_string(y));
    }
    
    return this->gameBoard[x][y];
}

bool Board::destinationInRange(glm::ivec2 destination, glm::ivec2 currentLoc) {
    if (!this->validTile(destination)) {
        throw std::range_error("Invalid destination");
    } else if (!this->validTile(currentLoc)) {
        throw std::range_error("Invalid currentLoc");
    }
    
    Creature *creature = this->get(currentLoc.x, currentLoc.y).creature();
    
    if (creature == nullptr) {
        throw std::invalid_argument("No creature at currentLoc");
    } else if (!this->get(destination.x, destination.y).passableByCreature(*creature)) {
        throw std::invalid_argument("Destination not passable by creature");
    }
    
    std::vector<Tile> tiles = this->getReachableTiles(this->get(currentLoc.x, currentLoc.y));
    
    for (int a = 0; a < tiles.size(); a++) {
        if (destination.x == tiles[a].x() && destination.y == tiles[a].y()) { //If the destination is within the reachable tiles, return true
            return true;
        }
    }
    
    //If it could not be found, return false
    return false;
}

bool Board::attackInRange(glm::ivec2 destination, glm::ivec2 currentLoc) {
    if (!this->validTile(destination)) {
        throw std::range_error("Invalid destination");
    } else if (!this->validTile(currentLoc)) {
        throw std::range_error("Invalid currentLoc");
    }
    
    Creature *creature = this->get(currentLoc.x, currentLoc.y).creature();
    
    if (creature == nullptr) {
        throw std::invalid_argument("No creature at currentLoc");
    } else if (this->get(destination.x, destination.y).creature() == nullptr) {
        throw std::invalid_argument("No cerature at attack location");
    }
    
    std::vector<Tile> tiles = this->getAttackableTiles(this->get(currentLoc.x, currentLoc.y));
    
    for (int a = 0; a < tiles.size(); a++) {
        if (destination.x == tiles[a].x() && destination.y == tiles[a].y()) { //If the destination is within the reachable tiles, return true
            return true;
        }
    }
    
    //If it could not be found, return false
    return false;
}

std::vector<Tile> Board::getReachableTiles(Tile creatureTile) {
    //Set the selected tile as the one inputted
    //    glm::ivec2 currentTile = glm::ivec2(creatureTile.x(), creatureTile.y());
    
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, int> > reachedTiles; //This is a vector containing the tiles found so far, along with the energy the creature has at that tile
        
        //Gets the tiles that are reachable by the creature
        reachedTiles.push_back(std::pair<Tile, int>(creatureTile, creatureTile.creature()->energy()));
        
#ifdef PATHFINDING_CONSOLE_OUTPUT
        std::cout << "pathfind" << std::endl;
#endif
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (int tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->get(tile.x(), tile.y() - 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() - 1))) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->get(tile.x() - 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x() - 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //South
                if (tile.y() < this->height(tile.x()) - 1) {
                    if (this->get(tile.x(), tile.y() + 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() + 1))) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //West
                if (tile.x() < this->width() - 1) {
                    if (this->get(tile.x() + 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x() + 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainMovementCost(this->get(tile.x(), tile.y()), this->get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
            }
        }
        
        //Now turn the reached tile vector of pairs into a vector of just tiles
        std::vector<Tile> reachedTileReturnVector;
        
        for (int tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            reachedTileReturnVector.push_back(reachedTiles[tileIterator].first);
        }
        
#ifdef PATHFINDING_CONSOLE_OUTPUT
        std::cout << "return success" << std::endl;
#endif
        return reachedTileReturnVector;
    }
}

/*
 * TO ADD:
 *
 * CHECK IF THE OCCUPYING CREATURE ON REACHABLE SQUARES ARE ATTACKABLE SPECIFICALLY BY THE CREATURE.
 */


//This function needs to be reworked for longer ranges. Perhaps, for each tile, check if there is a blocking obstacle in the way. Draw a line from origin to attack point, if it intersects with the boundaries of an obstacle the attack is not possible. Currently, projectiles can navigate around obstacles.
std::vector<Tile> Board::getAttackableTiles(Tile creatureTile) {
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        if (creature.energy() <= 0) {
            std::vector<Tile> emptyTileVector;
            return emptyTileVector;
        }
        
        std::vector<std::pair<Tile, int> > reachedTiles; //This is a vector containing the tiles found so far, along with the remaining range the attack has at that tile
        
        std::vector<Tile> attackableTiles; //A vector of the tiles that can be attacked
        
        reachedTiles.push_back(std::pair<Tile, int>(creatureTile, creatureTile.creature()->range()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (int tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If an attack at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->get(tile.x(), tile.y() - 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->getTerrainAttackCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x(), tile.y() - 1).occupied()) {
                        attackableTiles.push_back(this->get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->get(tile.x() - 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainAttackCost(this->get(tile.x(), tile.y()), this->get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x() - 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //South
                if (tile.y() < this->height(tile.x()) - 1) {
                    if (this->get(tile.x(), tile.y() + 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->getTerrainAttackCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x(), tile.y() + 1).occupied()) {
                        attackableTiles.push_back(this->get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //West
                if (tile.x() < this->width() - 1) {
                    if (this->get(tile.x() + 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainAttackCost(this->get(tile.x(), tile.y()), this->get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x() + 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
            }
        }
        
        for (int tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            attackableTiles.push_back(reachedTiles[tileIterator].first);
        }
        
        return attackableTiles;
    }
    
    /*if (creatureTile.creature() == nullptr) {
     std::vector<Tile> emptyTileVector;
     return emptyTileVector;
     } else {
     Creature creature = *creatureTile.creature();
     
     if (creature.energy() <= 0) {
     std::vector<Tile> emptyTileVector;
     return emptyTileVector;
     }
     
     std::vector<std::pair<Tile, int> > reachedTiles; //This is a vector containing the tiles found so far, along with the remaining range the attack has at that tile
     
     std::vector<Tile> attackableTiles; //A vector of the tiles that can be attacked
     
     reachedTiles.push_back(std::pair<Tile, int>(creatureTile, creatureTile.creature()->range()));
     
     //Keep pushing the vector back with new tiles, that the for loop will eventually go through
     for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
     if (reachedTiles[tileIterator].second > 0) { //If an attack at this spot would be able to continue to move further, expand in the four directions from that tile.
     
     Tile tile = reachedTiles[tileIterator].first;
     
     //North
     if (tile.y() > 0) {
     if (this->board.get(tile.x(), tile.y() - 1).passableByAttackStyle(creature)) {
     reachedTiles.push_back(std::pair<Tile, int>(this->board.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
     }
     if (this->board.get(tile.x(), tile.y() - 1).occupied()) {
     attackableTiles.push_back(this->board.get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
     }
     }
     
     //East
     if (tile.x() > 0) {
     if (this->board.get(tile.x() - 1, tile.y()).passableByAttackStyle(creature)) {
     reachedTiles.push_back(std::pair<Tile, int>(this->board.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
     }
     if (this->board.get(tile.x() - 1, tile.y()).occupied()) {
     attackableTiles.push_back(this->board.get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
     }
     }
     
     //South
     if (tile.y() < this->board.height(tile.x()) - 1) {
     if (this->board.get(tile.x(), tile.y() + 1).passableByAttackStyle(creature)) {
     reachedTiles.push_back(std::pair<Tile, int>(this->board.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
     }
     if (this->board.get(tile.x(), tile.y() + 1).occupied()) {
     attackableTiles.push_back(this->board.get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
     }
     }
     
     //West
     if (tile.x() < this->board.width() - 1) {
     if (this->board.get(tile.x() + 1, tile.y()).passableByAttackStyle(creature)) {
     reachedTiles.push_back(std::pair<Tile, int>(this->board.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
     }
     if (this->board.get(tile.x() + 1, tile.y()).occupied()) {
     attackableTiles.push_back(this->board.get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
     }
     }
     }
     }
     
     return attackableTiles;
     }
     */
}

std::vector<Tile> Board::getVisibleTiles(Tile creatureTile) {
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, int> > reachedTiles; //This is a vector containing the tiles found so far, along with the remaining range the attack has at that tile
        
        std::vector<Tile> visibleTiles; //A vector of the tiles that can be attacked
        
        reachedTiles.push_back(std::pair<Tile, int>(creatureTile, creatureTile.creature()->range()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (int tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If an attack at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->get(tile.x(), tile.y() - 1).passableByVision(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->getTerrainVisionCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x(), tile.y() - 1).occupied()) {
                        visibleTiles.push_back(this->get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->get(tile.x() - 1, tile.y()).passableByVision(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainVisionCost(this->get(tile.x(), tile.y()), this->get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x() - 1, tile.y()).occupied()) {
                        visibleTiles.push_back(this->get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //South
                if (tile.y() < this->height(tile.x()) - 1) {
                    if (this->get(tile.x(), tile.y() + 1).passableByVision(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->getTerrainVisionCost(this->get(tile.x(), tile.y()), this->get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x(), tile.y() + 1).occupied()) {
                        visibleTiles.push_back(this->get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //West
                if (tile.x() < this->width() - 1) {
                    if (this->get(tile.x() + 1, tile.y()).passableByVision(creature)) {
                        reachedTiles.push_back(std::pair<Tile, int>(this->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->getTerrainVisionCost(this->get(tile.x(), tile.y()), this->get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->get(tile.x() + 1, tile.y()).occupied()) {
                        visibleTiles.push_back(this->get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
            }
        }
        
        return visibleTiles;
    }
    
}

/*std::vector<Tile> Player::getAllVisibleTiles() {
 
 for (auto creature = board->creatures.begin(); creature != this->board->creatures.end(); creature++) {
 if (creature->controller() == playerNum) {
 
 //not done, but for each tile in getVisibleTiles, it gets pushed back to big vector. Then at the end all repeat tiles are removed.
 getVisibleTiles(this->&board[creature->x()][creature->y()]);
 }
 }
 
 
 }*/

bool Board::validTile(glm::ivec2 tilePos) {
    if (tilePos.x >= 0 && tilePos.x < this->width() && tilePos.y >= 0 && tilePos.y < this->height(tilePos.x))
        return true;
    else
        return false;
}

unsigned int Board::width() {
    return (unsigned int)this->gameBoard.size();
}

unsigned int Board::height(unsigned int x) {
    return (unsigned int)this->gameBoard[x].size();
}

std::string Board::serialize() {
    std::string str = "Board:" + std::to_string(this->gameBoard.size()) + ",";
    for (int x = 0; x < this->gameBoard.size(); x++) {
        str += std::to_string(this->gameBoard[x].size()) + ",";
        for (int y = 0; y < this->gameBoard[x].size(); y++) {
            str += this->gameBoard[x][y].serialize();
        }
    }
    str += "creatures=" + std::to_string(this->creatures.size()) + ",";
    for (auto a = this->creatures.begin(); a != this->creatures.end(); a++) {
        str += a->serialize();
    }
    str += "buildings=" + std::to_string(this->buildings.size()) + ",";
    for (auto a = this->buildings.begin(); a != this->buildings.end(); a++) {
        str += a->serialize();
    }
    return str + "-Board-";
}

Board Board::deserialize(std::string str) {
    str.erase(0, 6); //To erase "Board:"
    int width = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    std::vector<std::vector<Tile> > tiles (width);
    for (int x = 0; x < width; x++) {
        int height = std::stoi(str.substr(0, str.find_first_of(',')));
        str = str.substr(str.find_first_of(',') + 1);
        for (int y = 0; y < height; y++) {
            tiles[x].push_back(Tile::deserialize(str.substr(0, str.find("-Tile-"))));
            str = str.substr(str.find("-Tile-") + 6);
        }
    }
    
    Board board(tiles);
    
    str.erase(0, 10); //To erase "creatures="
    std::list<Creature> creatures;
    int numCreatures = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    for (int a = 0; a < numCreatures; a++) {
        Creature c = Creature::deserialize(str.substr(0, str.find("-Creature-")));
        board.setCreature(c.x(), c.y(), c);
        str = str.substr(str.find("-Creature-") + 10);
    }
    
    str.erase(0, 10); //To erase "buildings="
    std::list<Building> buildings;
    int numBuildings = std::stoi(str.substr(0, str.find_first_of(',')));
    str = str.substr(str.find_first_of(',') + 1);
    for (int a = 0; a < numBuildings; a++) {
        Building b = Building::deserialize(str.substr(0, str.find("-Building")));
        board.setBuilding(b.x(), b.y(), b);
        str = str.substr(str.find("-Building-") + 10);
    }
    
    return board;
}
