//
//  Game.cpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Game.hpp"

//Constructor

Game::Game(Board* board) : gameBoard(board) {
    for (int x = 0; x < this->gameBoard->width(); x++) {
        std::vector<std::array<int, 2> > boardColumnInfo;
        for (int y = 0; y < this->gameBoard->height(x); y++) {
            std::array<int, 2> tileData = { REGULAR, NO_HOVERING };
            
            boardColumnInfo.push_back(tileData);
        }
        this->boardInfo.push_back(boardColumnInfo);
    }
}

//Public member functions

//GLfloat Game::getDistance(glm::vec2 point1, glm::vec2 point2) {
//    return sqrtf(powf(point1.x - point2.x, 2.0) + powf(point1.y - point2.y, 2.0));
//}

bool Game::selectTile(int x, int y, unsigned int activePlayer) {
    glm::ivec2 passedInTile = glm::ivec2(x, y);
    
    if (passedInTile == NO_SELECTION) {
        this->selectedTile = NO_SELECTION;
    } else if (passedInTile == INTERFACE_BOX_SELECTION) {
        this->selectedTile = INTERFACE_BOX_SELECTION;
    } else if (x >= 0 && x < this->gameBoard->width() && y >= 0 && y < this->gameBoard->height(x)) { //Make sure the passed in tile is on the board
        
        //If there is a creature at that spot, properly select it. Otherwise just set it normally
        if (this->gameBoard->get(x, y).creature() != nullptr)
            this->selectCreature(x, y, activePlayer);
        else
            this->selectedTile = passedInTile;
        
    } else { //If selectedTile wasn't set, return false
        return false;
    }
    
    return true;
}

//Public get functions

Board* Game::board() {
    return this->gameBoard;
}

glm::ivec2 Game::tileSelected() {
    return this->selectedTile;
}

void Game::updateCreatures(float deltaTime, unsigned int activePlayer) {
    for (int x = 0; x < this->gameBoard->width(); x++) {
        for (int y = 0; y < this->gameBoard->height(x); y++) {
            
            Creature* creature = this->gameBoard->get(x, y).creature();
            
            if (creature != nullptr) {
                
                glm::ivec2 creatureLoc = glm::ivec2(x, y);
                
                unsigned int direction = creature->direction();
                
                if (direction == NORTH || direction == EAST) {
                    //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
                    
                    //If the creature is in the process of moving currently, continue to move it
                    if (creature->incrementOffset(deltaTime)) {
                        if (direction == NORTH) {
                            if (this->gameBoard->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.y -= 1;
                                
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                                this->selectCreature(x, y - 1, activePlayer);
#endif
                            }
                        } else if (direction == EAST) {
                            if (this->gameBoard->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.x -= 1;
                                
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                                this->selectCreature(x - 1, y, activePlayer);
#endif
                            }
                        }
                        
                    }
                } else if (direction == SOUTH || direction == WEST) {
                    
                    if (creature->incrementOffset(deltaTime)) {
                        
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                        this->selectCreature(x, y, activePlayer);
#endif
                    }
                }
                
                if (creature->directions.size() > 0 && creature->offset() == 0.0) {
                    
                    //Get the new direction that the creature will be travelling in.
                    unsigned int newDirection = creature->directions.front();
                    
                    //Now that this direction is being dealt with, we can get rid of it from the directions left for the creature to go in.
                    creature->directions.pop();
                    
                    this->moveAdjacent(creatureLoc.x, creatureLoc.y, newDirection, deltaTime);
                }
            }
        }
    }
}

void Game::updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int activePlayer) {
    //Update all tiles other than the one where the mouse is to have no hovering
    for (int x = 0; x < this->gameBoard->width(); x++) {
        for (int y = 0; y < this->gameBoard->height(x); y++) {
            this->boardInfo[x][y][TILE_HOVER] = NO_HOVERING;
        }
    }
    
    if (mousePos != NO_SELECTION)
        this->boardInfo[mousePos.x][mousePos.y][TILE_HOVER] = HOVERING;
    
    if (mouseDown) {
        if (mousePos == NO_SELECTION) {
            //Reset all tiles if the mouse clicked out of the screen
            for (int x = 0; x < this->gameBoard->width(); x++) {
                for (int y = 0; y < this->gameBoard->height(x); y++) {
                    this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                }
            }
        }
        
        //Reset the tile (and others) if the current tile is clicked again
        else if (mousePos == this->selectedTile) {
            
            //Goes through all tiles and sets them to REGULAR
            for (int x = 0; x < this->gameBoard->width(); x++) {
                for (int y = 0; y < this->gameBoard->height(x); y++) {
                    this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                }
            }
            
            //Set selectedTile to null results
            this->selectedTile = NO_SELECTION;
        }
        
        //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
        else if (this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] == REGULAR) {
            
            //Reset all tiles (this one is highlighted after)
            for (int x = 0; x < this->gameBoard->width(); x++) {
                for (int y = 0; y < this->gameBoard->height(x); y++) {
                    this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                }
            }
            
            //Select this new tile
            this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] = SELECTED;
            
            //If the selected tile is a creature, highlight reachable tiles and update the creature's direction
            
            if (this->gameBoard->get(mousePos.x, mousePos.y).creature() != nullptr && this->gameBoard->get(mousePos.x, mousePos.y).creature()->controller() == activePlayer) {
                std::vector<Tile> reachableTiles = getReachableTiles(this->gameBoard->get(mousePos.x, mousePos.y));
                
                Creature creature = *this->gameBoard->get(mousePos.x, mousePos.y).creature();
                for (int a = 0; a < reachableTiles.size(); a++) {
                    if (this->gameBoard->get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(creature)) {
                        this->boardInfo[reachableTiles[a].x()][reachableTiles[a].y()][TILE_STYLE] = REACHABLE;
                    }
                }
                
                std::vector<Tile> attackableTiles = getAttackableTiles(this->gameBoard->get(mousePos.x, mousePos.y));
                
                for (int a = 0; a < attackableTiles.size(); a++) {
                    
                    //If there is a creature on the tile, controlled by an opponent, make it attackable
                    if (this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).creature() != nullptr && this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).creature()->controller() != activePlayer)
                        
                        if (creature.energy() > 0)
                            this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
                    
                    //If there is a building on the tile, controlled by an opponent, make it attackable
                    if (this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).building() != nullptr && this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).building()->controller() != activePlayer)
                        
                        if (creature.energy() > 0)
                            this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
                }
            }
            
            this->selectedTile = mousePos;
        }
        
        //Selecting selectable tiles
        else if (this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] == REACHABLE) {
            
            //Move the creature to the selected tile
            if (this->gameBoard->get(this->selectedTile.x, this->selectedTile.y).creature() != nullptr) {
                std::vector<unsigned int> directions = this->getPath(this->selectedTile.x, this->selectedTile.y, mousePos.x, mousePos.y);
                
                for (int a = 0; a < directions.size(); a++) {
                    this->gameBoard->get(this->selectedTile.x, this->selectedTile.y).creature()->directions.push(directions[a]);
                    if (a == 0)
                        this->gameBoard->setDirection(this->selectedTile.x, this->selectedTile.y, directions[a]);
                }
                
                //Reset all tiles
                for (int x = 0; x < this->gameBoard->width(); x++) {
                    for (int y = 0; y < this->gameBoard->height(x); y++) {
                        this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                    }
                }
                
                this->selectedTile = NO_SELECTION;
            }
            
            //Create the creature from the building
            else if (this->gameBoard->get(this->selectedTile.x, this->selectedTile.y).building() != nullptr) {
                Creature newCreature(mousePos.x, mousePos.y, Human, 100, 4, 30, LightMelee, 1, 1, 1, NORTH, activePlayer);
                
                if (this->gameBoard->get(mousePos.x, mousePos.y).passableByCreature(newCreature)) {
                    this->gameBoard->setCreature(mousePos.x, mousePos.y, newCreature);
                }
                
                //Reset all tiles
                for (int x = 0; x < this->gameBoard->width(); x++) {
                    for (int y = 0; y < this->gameBoard->height(x); y++) {
                        this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                    }
                }
                
                this->selectedTile = NO_SELECTION;
            }
        }
        
        //Attacking
        else if (this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] == ATTACKABLE) {
            
            glm::ivec2 attacker = glm::ivec2(this->selectedTile.x, this->selectedTile.y);
            glm::ivec2 defender = glm::ivec2(mousePos.x, mousePos.y);
            
            if (this->gameBoard->tileDistances(attacker.x, attacker.y, defender.x, defender.y) <= this->gameBoard->get(attacker.x, attacker.y).creature()->range()) {
                
                int attackDamage = 0;
                int defendDamage = 0;
                
                this->gameBoard->initiateCombat(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
                
                //Set the damage data on the defending square equal to damage dealt by the attacker
                this->gameBoard->setDamage(defender.x, defender.y, attackDamage, glfwGetTime());
                
                //Set the damage data on the attacking square equal to damage dealt by the defender
                this->gameBoard->setDamage(attacker.x, attacker.y, defendDamage, glfwGetTime());
                
                //Reset all tiles
                for (int x = 0; x < this->gameBoard->width(); x++) {
                    for (int y = 0; y < this->gameBoard->height(x); y++) {
                        this->boardInfo[x][y][TILE_STYLE] = REGULAR;
                    }
                }
                
                this->selectedTile = NO_SELECTION;
                
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                //If the attacker died, nothing will happen and the function will return false
                this->selectCreature(attacker.x, attacker.y, activePlayer);
#endif
            }
        }
    }
}

//Private member functions

bool Game::moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime) {
    //Return false if there is no creature at the designated spot to move
    if (this->gameBoard->get(x, y).creature() == nullptr)
        return false;
    
    //Check if move goes beyond map
    int newX, newY;
    
    if (direction == NORTH) {
        newX = x;
        newY = y - 1;
        
        if (newY < 0)
            return false;
    } else if (direction == WEST) {
        newX = x + 1;
        newY = y;
        
        if (newX >= this->gameBoard->width())
            return false;
    } else if (direction == SOUTH) {
        newX = x;
        newY = y + 1;
        
        if (newY >= this->gameBoard->height(x))
            return false;
    } else if (direction == EAST) {
        newX = x - 1;
        newY = y;
        
        if (newX < 0)
            return false;
    }
    
    //Passable Check
    if (!this->gameBoard->get(newX, newY).passableByCreature(*this->gameBoard->get(x, y).creature())) {
        return false;
    }
    
    //Set the direction that was found at the selected creature
    this->gameBoard->setDirection(x, y, direction);
    
    //If the tile is going to be moving up (visually on the screen) slowly move the tile from the previous location to the new one
    //For these directions, the creature is moved after, in the function that updates the offset data
    if (direction == NORTH || direction == EAST)
        this->gameBoard->get(x, y).creature()->initiateMovementOffset(deltaTime);
    
    //If it's going down, instead move it to the next square and slowly move it from that spot. This keeps it from being drawn under the tile it's going to
    //For these directions, the creature is moved here, and then the offset is slowly updated to follow
    if (direction == SOUTH || direction == WEST) {
        unsigned int tile; //The location in the data array
        
        if (direction == SOUTH) {
            tile = (x * this->gameBoard->width()) + (y + 1); //One row below
        } else if (direction == WEST) {
            tile = ((x + 1) * this->gameBoard->width()) + y; //One tile further
        }
        
        if (tile < NUMBER_OF_TILES) {
            this->gameBoard->get(x, y).creature()->initiateMovementOffset(deltaTime);
            
            this->gameBoard->moveCreatureByDirection(x, y, direction);
        } else {
            return false;
        }
    }
    return true;
}

bool Game::selectCreature(unsigned int x, unsigned int y, unsigned int activePlayer) {
    if (x >= this->gameBoard->width()) //No selecting happens if the x is out of range
        return false;
    
    if (y >= this->gameBoard->height(x)) //No selecting happens if the y is out of range
        return false;
    
    if (this->gameBoard->get(x, y).creature() == nullptr) //No selecting happens if there is no creature at the selected location
        return false;
    
    Creature* creature = this->gameBoard->get(x, y).creature();
    
    if (creature->directions.size() == 0) {
        this->selectedTile = glm::vec2(x, y); //Set the selected tile to this location
        
        this->boardInfo[x][y][TILE_STYLE] = SELECTED;
        
        std::vector<Tile> reachableTiles = getReachableTiles(this->gameBoard->get(x, y));
        
        Creature creature = *this->gameBoard->get(x, y).creature();
        for (int a = 0; a < reachableTiles.size(); a++) {
            if (this->gameBoard->get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(creature)) {
                this->boardInfo[reachableTiles[a].x()][reachableTiles[a].y()][TILE_STYLE] = REACHABLE;
            }
        }
        
        std::vector<Tile> attackableTiles = getAttackableTiles(this->gameBoard->get(x, y));
        
        for (int a = 0; a < attackableTiles.size(); a++) {
            
            //If there is a creature on the tile, controlled by an opponent, make it attackable
            if (this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).creature() != nullptr && this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).creature()->controller() != activePlayer)
                
                if (creature.energy() > 0)
                    this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
            
            //If there is a building on the tile, controlled by an opponent, make it attackable
            if (this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).building() != nullptr && this->gameBoard->get(attackableTiles[a].x(), attackableTiles[a].y()).building()->controller() != activePlayer)
                
                if (creature.energy() > 0)
                    this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
        }
    }
    
    return true;
}

std::vector<Tile> Game::getReachableTiles(Tile creatureTile) {
    //Set the selected tile as the one inputted
    //    glm::ivec2 currentTile = glm::ivec2(creatureTile.x(), creatureTile.y());
    
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, GLint> > reachedTiles; //This is a vector containing the tiles found so far, along with the energy the creature has at that tile
        
        //Gets the tiles that are reachable by the creature
        reachedTiles.push_back(std::pair<Tile, GLint>(creatureTile, creatureTile.creature()->energy()));
        
#ifdef PATHFINDING_CONSOLE_OUTPUT
        std::cout << "pathfind" << std::endl;
#endif
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->gameBoard->get(tile.x(), tile.y() - 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() - 1))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard->get(tile.x() - 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() - 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard->height(tile.x()) - 1) {
                    if (this->gameBoard->get(tile.x(), tile.y() + 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() + 1))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //West
                if (tile.x() < this->gameBoard->width() - 1) {
                    if (this->gameBoard->get(tile.x() + 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() + 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->gameBoard->getTerrainMovementCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
            }
        }
        
        //Now turn the reached tile vector of pairs into a vector of just tiles
        std::vector<Tile> reachedTileReturnVector;
        
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
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


std::vector<Tile> Game::getAttackableTiles(Tile creatureTile) {
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, GLint> > reachedTiles; //This is a vector containing the tiles found so far, along with the remaining range the attack has at that tile
        
        std::vector<Tile> attackableTiles; //A vector of the tiles that can be attacked
        
        reachedTiles.push_back(std::pair<Tile, GLint>(creatureTile, creatureTile.creature()->range()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If an attack at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->gameBoard->get(tile.x(), tile.y() - 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->gameBoard->getTerrainAttackCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->gameBoard->get(tile.x(), tile.y() - 1).occupied()) {
                        attackableTiles.push_back(this->gameBoard->get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard->get(tile.x() - 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->gameBoard->getTerrainAttackCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->gameBoard->get(tile.x() - 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->gameBoard->get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard->height(tile.x()) - 1) {
                    if (this->gameBoard->get(tile.x(), tile.y() + 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->gameBoard->getTerrainAttackCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->gameBoard->get(tile.x(), tile.y() + 1).occupied()) {
                        attackableTiles.push_back(this->gameBoard->get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //West
                if (tile.x() < this->gameBoard->width() - 1) {
                    if (this->gameBoard->get(tile.x() + 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->gameBoard->getTerrainAttackCost(this->gameBoard->get(tile.x(), tile.y()), this->gameBoard->get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->gameBoard->get(tile.x() + 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->gameBoard->get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
            }
        }
        
        return attackableTiles;
    }
}

std::vector<GLuint> Game::getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY) {
    if (x >= this->gameBoard->width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (y >= this->gameBoard->height(x)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationX >= this->gameBoard->width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationY >= this->gameBoard->height(destinationX)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (this->gameBoard->get(x, y).creature() == nullptr) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (!this->gameBoard->get(destinationX, destinationY).passableByCreature(*this->gameBoard->get(x, y).creature())) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    }
    
    Creature creature = *this->gameBoard->get(x, y).creature();
    
    std::queue<std::vector<std::pair<GLuint, GLuint> > > possiblePaths;
    
    std::vector<std::pair<GLuint, GLuint> > firstTile { std::pair<GLuint, GLuint>(x, y) };
    
    possiblePaths.push(firstTile);
    
    std::vector<std::pair<GLuint, GLuint> > foundPath;
    
    while (possiblePaths.size() > 0) {
        
        std::vector<std::pair<GLuint, GLuint> > path = possiblePaths.front();
        
        if (path.back().first == destinationX && path.back().second == destinationY) {
            foundPath = path;
            break;
        }
        
        if (possiblePaths.front().size() <= creature.energy()) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
            
            std::pair<GLuint, GLuint> tile = path.back();
            
            //North
            if (tile.second > 0) {
                if (this->gameBoard->get(tile.first, tile.second - 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second - 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //East
            if (tile.first > 0) {
                if (this->gameBoard->get(tile.first - 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first - 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
            
            //South
            if (tile.second < this->gameBoard->height(tile.first) - 1) {
                if (this->gameBoard->get(tile.first, tile.second + 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second + 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //West
            if (tile.first < this->gameBoard->width() - 1) {
                if (this->gameBoard->get(tile.first + 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first + 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
        }
        
        possiblePaths.pop();
    }
    
    std::vector<GLuint> directions;
    
    for (GLuint a = 0; a < foundPath.size(); a++) {
        if (foundPath[a].first == x && foundPath[a].second == y) {
            continue; //First spot is the original location
        }
        
        std::pair<GLuint, GLuint> previousTile = a == 0 ? std::pair<GLuint, GLuint>(x, y) : foundPath[a - 1];
        
        if (foundPath[a].second < previousTile.second && foundPath[a].first == previousTile.first) {
            directions.push_back(NORTH);
        } else if (foundPath[a].first < previousTile.first && foundPath[a].second == previousTile.second) {
            directions.push_back(EAST);
        } else if (foundPath[a].second > previousTile.second && foundPath[a].first == previousTile.first) {
            directions.push_back(SOUTH);
        } else if (foundPath[a].first > previousTile.first && foundPath[a].second == previousTile.second) {
            directions.push_back(WEST);
        }
    }
    
    return directions;
}

glm::vec3 Game::tileColor(unsigned int x, unsigned int y) {
    if (x >= this->gameBoard->width())
        throw std::range_error("X out of range");
    if (y >= this->gameBoard->height(x))
        throw std::range_error("Y out of range");
    
    int style = this->boardInfo[x][y][TILE_STYLE];
    int hover = this->boardInfo[x][y][TILE_HOVER];
    
    if (style == REGULAR)
        return hover == NO_HOVERING ? WHITE : WHITE * HOVER_EFFECT;
    else if (style == SELECTED)
        return hover == NO_HOVERING ? GREY : GREY * HOVER_EFFECT;
    else if (style == ATTACKABLE)
        return hover == NO_HOVERING ? RED : RED * HOVER_EFFECT;
    else if (style == REACHABLE)
        return hover == NO_HOVERING ? GREEN : GREEN * HOVER_EFFECT;
    
    //Something went wrong. Return White to have an unaltered color
    return WHITE;
}
