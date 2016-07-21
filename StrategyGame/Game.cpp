//
//  Game.cpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Game.hpp"

//Constructor

Game::Game(Board board) : gameBoard(board) {
    
}

//Public member functions

GLfloat Game::getDistance(glm::vec2 point1, glm::vec2 point2) {
    return sqrtf(powf(point1.x - point2.x, 2.0) + powf(point1.y - point2.y, 2.0));
}

//Public get functions
Board* Game::board() {
    return &this->gameBoard;
}

//Private member functions

void Game::updateCreatures(float deltaTime) {
    for (int x = 0; x < this->gameBoard.width(); x++) {
        for (int y = 0; y < this->gameBoard.height(x); y++) {
            
            Creature* creature = this->gameBoard.get(x, y).creature();
            
            if (creature != nullptr) {
                
                glm::ivec2 creatureLoc = glm::ivec2(x, y);
                
                unsigned int direction = creature->direction();
                
                if (direction == NORTH || direction == EAST) {
                    //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
                    
                    //If the creature is in the process of moving currently, continue to move it
                    creature->incrementOffset(deltaTime);
                    
                    if (creature->readyToMove()) {
                        if (direction == NORTH) {
                            if (this->gameBoard.moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.y -= 1;
                            }
                        } else if (direction == EAST) {
                            if (this->gameBoard.moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.x -= 1;
                            }
                        }
                        
                    }
                } else if (direction == SOUTH || direction == WEST) {
                    
                    creature->incrementOffset(deltaTime);
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

void Game::updateSelected(bool *mouseDown) {
    glm::ivec2 mousePos;
    
    mousePos = mouseTile();
    
    if (mousePos == INTERFACE_BOX_SELECTION) {
        //Don't alter the selected tile if the interface box has been clicked
        
        //Make mouseDown true again so the interface box can check the mouse click location
        *mouseDown = true;
        
    } else if (mousePos == NO_SELECTION) {
        //Reset all tiles if the mouse clicked out of the screen
        for (int x = 0; x < this->gameBoard.width(); x++) {
            for (int y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
    }
    
    //Reset the tile (and others) if the current tile is clicked again
    else if (mousePos == this->selectedTile) {
        
        //Goes through all tiles and sets them to regular
        for (int x = 0; x < this->gameBoard.width(); x++) {
            for (int y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
        
        //Set selectedTile to null results
        this->selectedTile = NO_SELECTION;
    }
    
    //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
    else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == Regular) {
        
        //Reset all tiles (this one is highlighted after)
        for (int x = 0; x < this->gameBoard.width(); x++) {
            for (int y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
        
        //Select this new tile
        this->gameBoard.setStyle(mousePos.x, mousePos.y, Selected);
        
        //If the selected tile is a creature, highlight reachable tiles and update the creature's direction
        
        if (this->gameBoard.get(mousePos.x, mousePos.y).creature() != nullptr && this->gameBoard.get(mousePos.x, mousePos.y).creature()->controller() == activePlayer) {
            std::vector<Tile> reachableTiles = getReachableTiles(this->gameBoard.get(mousePos.x, mousePos.y));
            
            Creature creature = *this->gameBoard.get(mousePos.x, mousePos.y).creature();
            for (int a = 0; a < reachableTiles.size(); a++) {
                if (this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(creature)) {
                    this->gameBoard.setStyle(reachableTiles[a].x(), reachableTiles[a].y(), Reachable);
                } else if (this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).creature() != nullptr && this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).creature()->controller() != this->activePlayer) {
                    
                    //Only set the tile to be attackable if it is within the creature's range
                    if (this->gameBoard.tileDistances(mousePos.x, mousePos.y, reachableTiles[a].x(), reachableTiles[a].y()) <= creature.range())
                        this->gameBoard.setStyle(reachableTiles[a].x(), reachableTiles[a].y(), AttackableAdj);
                }
            }
        }
        
        this->selectedTile = mousePos;
    }
    
    //Movement
    else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == Reachable) {
        
        std::vector<unsigned int> directions = this->getPath(this->selectedTile.x, this->selectedTile.y, mousePos.x, mousePos.y);
        
        for (int a = 0; a < directions.size(); a++) {
            this->gameBoard.get(this->selectedTile.x, this->selectedTile.y).creature()->directions.push(directions[a]);
            if (a == 0)
                this->gameBoard.setDirection(this->selectedTile.x, this->selectedTile.y, directions[a]);
        }
        
        //Reset all tiles
        for (int x = 0; x < this->gameBoard.width(); x++) {
            for (int y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
        
        this->selectedTile = NO_SELECTION;
    }
    
    //Attacking
    else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == AttackableAdj) {
        
        glm::ivec2 attacker = glm::ivec2(this->selectedTile.x, this->selectedTile.y);
        glm::ivec2 defender = glm::ivec2(mousePos.x, mousePos.y);
        
        if (this->gameBoard.tileDistances(attacker.x, attacker.y, defender.x, defender.y) <= this->gameBoard.get(attacker.x, attacker.y).creature()->range()) {
            
            int attackDamage = 0;
            int defendDamage = 0;
            
            this->gameBoard.attack(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
            
            //Set the damage data on the defending square equal to damage dealt by the attacker
            this->gameBoard.setDamage(defender.x, defender.y, attackDamage, glfwGetTime());
            
            //Set the damage data on the attacking square equal to damage dealt by the defender
            this->gameBoard.setDamage(attacker.x, attacker.y, defendDamage, glfwGetTime());
            
            //Reset all tiles
            for (int x = 0; x < this->gameBoard.width(); x++) {
                for (int y = 0; y < this->gameBoard.height(x); y++) {
                    this->gameBoard.setStyle(x, y, Regular);
                }
            }
            
            this->selectedTile = NO_SELECTION;
        }
    }
}

bool Game::moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime) {
    //Return false if there is no creature at the designated spot to move
    if (this->gameBoard.get(x, y).creature() == nullptr)
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
        
        if (newX >= this->gameBoard.width())
            return false;
    } else if (direction == SOUTH) {
        newX = x;
        newY = y + 1;
        
        if (newY >= this->gameBoard.height(x))
            return false;
    } else if (direction == EAST) {
        newX = x - 1;
        newY = y;
        
        if (newX < 0)
            return false;
    }
    
    //Passable Check
    if (!this->gameBoard.get(newX, newY).passableByCreature(*this->gameBoard.get(x, y).creature())) {
        return false;
    }
    
    //Set the direction that was found at the selected creature
    this->gameBoard.setDirection(x, y, direction);
    
    //If the tile is going to be moving up (visually on the screen) slowly move the tile from the previous location to the new one
    //For these directions, the creature is moved after, in the function that updates the offset data
    if (direction == NORTH || direction == EAST)
        this->gameBoard.get(x, y).creature()->initiateMovementOffset(deltaTime);
    
    //If it's going down, instead move it to the next square and slowly move it from that spot. This keeps it from being drawn under the tile it's going to
    //For these directions, the creature is moved here, and then the offset is slowly updated to follow
    if (direction == SOUTH || direction == WEST) {
        unsigned int tile; //The location in the data array
        
        if (direction == SOUTH) {
            tile = (x * this->gameBoard.width()) + (y + 1); //One row below
        } else if (direction == WEST) {
            tile = ((x + 1) * this->gameBoard.width()) + y; //One tile further
        }
        
        if (tile < NUMBER_OF_TILES) {
            this->gameBoard.get(x, y).creature()->initiateMovementOffset(deltaTime);
            
            this->gameBoard.moveCreatureByDirection(x, y, direction);
        }
    }
    return true;
}

void Game::incrementActivePlayer() {
    this->activePlayer++;
    
    if (this->activePlayer >= NUMBER_OF_PLAYERS)
        this->activePlayer = 0;
}

/*
 * TO ADD:
 *
 * CHECK IF THE OCCUPYING CREATURE ON REACHABLE SQUARES ARE ATTACKABLE SPECIFICALLY BY THE CREATURE.
 */
std::vector<Tile> Game::getReachableTiles (Tile creatureTile) {
    //Set the selected tile as the one inputted
    //    glm::ivec2 currentTile = glm::ivec2(creatureTile.x(), creatureTile.y());
    
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, GLint> > reachedTiles; //This is a vector containing the tiles found so far, along with the energy the creature has at that tile
        
        reachedTiles.push_back(std::pair<Tile, GLint>(creatureTile, creatureTile.creature()->energy()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->gameBoard.get(tile.x(), tile.y() - 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() - 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard.get(tile.x() - 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() - 1, tile.y()).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard.height(tile.x()) - 1) {
                    if (this->gameBoard.get(tile.x(), tile.y() + 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() + 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //West
                if (tile.y() < this->gameBoard.width() - 1) {
                    if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
            }
        }
        
        //Now turn the reached tile vector of pairs into a vector of just tiles
        std::vector<Tile> reachedTileReturnVector;
        
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            reachedTileReturnVector.push_back(reachedTiles[tileIterator].first);
        }
        return reachedTileReturnVector;
    }
}

std::vector<GLuint> Game::getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY) {
    if (x >= this->gameBoard.width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (y >= this->gameBoard.height(x)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationX >= this->gameBoard.width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationY >= this->gameBoard.height(destinationX)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (this->gameBoard.get(x, y).creature() == nullptr) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (!this->gameBoard.get(destinationX, destinationY).passableByCreature(*this->gameBoard.get(x, y).creature())) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    }
    
    Creature creature = *this->gameBoard.get(x, y).creature();
    
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
                if (this->gameBoard.get(tile.first, tile.second - 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second - 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //East
            if (tile.first > 0) {
                if (this->gameBoard.get(tile.first - 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first - 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
            
            //South
            if (tile.second < this->gameBoard.height(tile.first) - 1) {
                if (this->gameBoard.get(tile.first, tile.second + 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second + 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //West
            if (tile.first < this->gameBoard.width() - 1) {
                if (this->gameBoard.get(tile.first + 1, tile.second).passableByCreature(creature)) {
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