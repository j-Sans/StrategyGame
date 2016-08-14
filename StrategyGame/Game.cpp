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

void Game::nextTurn() {
    this->incrementActivePlayer();
    
    //Increment the turn if a full player cycle has occurred
    if (this->currentActivePlayer == 0)
        turn++;
    
    //Iterate through the entire board and reset style and energy.
    for (GLuint x = 0; x < this->gameBoard.width(); x++) {
        for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
            this->gameBoard.setStyle(x, y, Regular);
            Creature* creature = this->gameBoard.get(x, y).creature();
            if (creature != nullptr)
                creature->resetEnergy();
        }
    }
    
#ifdef TURN_CONSOLE_OUTPUT
    std::cout << std::endl << std::endl << "--------------------" << std::endl << "Player " << this->currentActivePlayer + 1 << "'s turn" << std::endl << std::endl;
#endif
}

//Public get functions

Board* Game::board() {
    return &this->gameBoard;
}

unsigned int Game::activePlayer() {
    return this->currentActivePlayer;
}

glm::ivec2 Game::tileSelected() {
    return this->selectedTile;
}

void Game::updateCreatures(float deltaTime) {
    for (int x = 0; x < this->gameBoard.width(); x++) {
        for (int y = 0; y < this->gameBoard.height(x); y++) {
            
            Creature* creature = this->gameBoard.get(x, y).creature();
            
            /*if (creature != nullptr) {
                
                GLuint direction = creature->direction();
                
                glm::ivec2 creatureLoc;
                
                if (direction == NORTH || direction == EAST) {
                    //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
                    
                    //If the creature is in the process of moving currently, continue to move it
                    creature->incrementOffset(deltaTime);
                    
                    if (creature->readyToMove()) {
                        if (direction == NORTH) {
                            if (board()->moveCreatureByDirection(x, y, direction)) {
                                creatureLoc.y -= 1;
                            }
                        } else if (direction == EAST) {
                            if (board()->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.x -= 1;
                            }
                        }
                        
                    }
                } else if (direction == SOUTH || direction == WEST) {
                    
                    creature->incrementOffset(deltaTime);
                }
                
                if (creature->directions.size() > 0 && creature->offset() == 0.0) {
                    
                    //Get the new direction that the creature will be travelling in.
                    GLuint newDirection = creature->directions.front();
                    
                    //Now that this direction is being dealt with, we can get rid of it from the directions left for the creature to go in.
                    creature->directions.pop();
                    
                    this->moveAdjacent(creatureLoc.x, creatureLoc.y, newDirection);
                }
            }*/
            
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

void Game::updateSelected(bool *mouseDown, glm::vec2 cursorPos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]) {
    glm::ivec2 mousePos;
    
    mousePos = mouseTile(cursorPos, windowSize, tileCenters);
    
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
        
        if (this->gameBoard.get(mousePos.x, mousePos.y).creature() != nullptr && this->gameBoard.get(mousePos.x, mousePos.y).creature()->controller() == currentActivePlayer) {
            std::vector<Tile> reachableTiles = getReachableTiles(this->gameBoard.get(mousePos.x, mousePos.y));
            
            Creature creature = *this->gameBoard.get(mousePos.x, mousePos.y).creature();
            for (int a = 0; a < reachableTiles.size(); a++) {
                if (this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(creature)) {
                    this->gameBoard.setStyle(reachableTiles[a].x(), reachableTiles[a].y(), Reachable);
                } /*else {
                    if (this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).creature() != nullptr && this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).creature()->controller() != this->currentActivePlayer) {
                    
                        //Only set the tile to be attackable if it is within the creature's range
                        if (this->gameBoard.tileDistances(mousePos.x, mousePos.y, reachableTiles[a].x(), reachableTiles[a].y()) <= creature.range())
                            this->gameBoard.setStyle(reachableTiles[a].x(), reachableTiles[a].y(), AttackableAdj);
                    }
                    
                    if (this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).building() != nullptr && this->gameBoard.get(reachableTiles[a].x(), reachableTiles[a].y()).building()->controller() != this->currentActivePlayer) {
                    
                        //Only set the tile to be attackable if it is within the creature's range
                        if (this->gameBoard.tileDistances(mousePos.x, mousePos.y, reachableTiles[a].x(), reachableTiles[a].y()) <= creature.range())
                            this->gameBoard.setStyle(reachableTiles[a].x(), reachableTiles[a].y(), AttackableAdj);
                    }
                }*/
            }
            
            std::vector<Tile> attackableTiles = getAttackableTiles(this->gameBoard.get(mousePos.x, mousePos.y));
            
            //move this later
            for (int a = 0; a < attackableTiles.size(); a++) {
                
                //If there is a creature on the tile, controlled by an opponent, make it attackable
                if (this->gameBoard.get(attackableTiles[a].x(), attackableTiles[a].y()).creature() != nullptr && this->gameBoard.get(attackableTiles[a].x(), attackableTiles[a].y()).creature()->controller() != this->currentActivePlayer)
                    
                    if (creature.energy() > 0)
                        this->gameBoard.setStyle(attackableTiles[a].x(), attackableTiles[a].y(), AttackableAdj);
                
                //If there is a building on the tile, controlled by an opponent, make it attackable
                if (this->gameBoard.get(attackableTiles[a].x(), attackableTiles[a].y()).building() != nullptr && this->gameBoard.get(attackableTiles[a].x(), attackableTiles[a].y()).building()->controller() != this->currentActivePlayer)
                    
                    if (creature.energy() > 0)
                        this->gameBoard.setStyle(attackableTiles[a].x(), attackableTiles[a].y(), AttackableAdj);
                
                std::cout << attackableTiles[a].x() << ", " << attackableTiles[a].y() << '\n';
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

//Private member functions

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
    this->currentActivePlayer++;
    
    if (this->currentActivePlayer >= NUMBER_OF_PLAYERS)
        this->currentActivePlayer = 0;
}

glm::ivec2 Game::mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]) {
    GLint tileIndex = -1; //The tile index where the mouse was clicked. Initialized as -1 to mean no index found
    
    //If x is in the last sixth or the first sixth, ignore the click because the interface boxes were clicked
    if (mousePos.x > (windowSize.x * 5.0 / 6.0) || mousePos.x < (windowSize.x / 6.0))
        return INTERFACE_BOX_SELECTION;
    
    //Only the middle 2/3 of the screen is the board, so make the start of that section 0
    mousePos.x -= (windowSize.x / 6.0);
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    mousePos.x *= (3.0 / 2.0);
    
    //Do the same for y, except that only the bottom 1/4 of the screen is not part of the board
    if (mousePos.y > (windowSize.y * 3.0 / 4.0))
        return INTERFACE_BOX_SELECTION;
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    mousePos.y *= (4.0 / 3.0);
    
    //Make mousePos between 0 and 1 by dividing the position by the maximum position (width or height)
    mousePos.x /= windowSize.x;
    mousePos.y /= windowSize.y;
    
    //Now make it 0 to 2 by doubling it
    mousePos.x *= 2.0f;
    mousePos.y *= 2.0f;
    
    //Now subtract 1 to get it between -1 and 1
    mousePos.x -= 1.0f;
    mousePos.y -= 1.0f;
    
    //So that -1 is the bottom of the screen, not the top
    mousePos.y = -mousePos.y;
    
    /*for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
    }*/
    
    //The distance from one point to the horizontal point and the vertical point:
    
    //The points diagonally above and below each vertex become horizontal and vertical after rotation. To find them, find the point below the vertex and add one and subtract one.
    
    if (BOARD_WIDTH * BOARD_WIDTH < BOARD_WIDTH + 1) { //In case finding the distances (just below) would cause a bad access
        throw std::length_error("Board too small");
    }
    
    GLfloat distance1 = Game::getDistance(tileCenters[0], tileCenters[0 + BOARD_WIDTH + 1]); //Diagonal down and to the right
    GLfloat distance2 = Game::getDistance(tileCenters[1], tileCenters[1 + BOARD_WIDTH - 1]); //Diagonal down and to the left
    
    //Distance horizontally is double the distance of the vertical one because it was compressed vertically.
    //The horizontal distance is the max of the above distances, and the vertical distance the minimum
    
    GLfloat verticalDistance = fminf(distance1, distance2);
    GLfloat horizontalDistance = fmaxf(distance1, distance2);
    
    //For every point, check if it is within the boundaries of the respective diamond's bounds, by finding the 4 bounding lines of that rectange
    
    GLfloat slope = verticalDistance / horizontalDistance; // = rise / run
    
    //Using line equation:
    // y = slope ( x - h ) + k
    //Where (h,k) is a point on the line
    
    for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
        glm::vec2 center = glm::vec2(tileCenters[index].x, tileCenters[index].y);
        
        bool pointInIndex = true;
        
        //Lower left inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y > ( -slope ) ( x - h ) + k
        // (h,k) is the point below the center
        
        GLfloat h = center.x;
        GLfloat k = center.y - (verticalDistance / 2.0);
        
        if (mousePos.y < ( -slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
            continue;
        }
        
        //Lower right inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y > ( slope ) ( x - h ) + k
        // (h,k) is the point below the center, the same as previously
        
        if (mousePos.y < ( slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
            continue;
        }
        
        //Upper left inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y < ( slope ) ( x - h ) + k
        // (h,k) is the point above the center
        
        h = center.x; //h stays the same
        k = center.y + (verticalDistance / 2.0);
        
        if (mousePos.y > ( slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
            continue;
        }
        //Upper right inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y < ( -slope ) ( x - h ) + k
        // (h,k) is the point above the center, the same as previously
        
        if (mousePos.y > ( -slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
            continue;
        }
        
        if (pointInIndex) { //The point was in bounds
            tileIndex = index;
            break; //Point found, no need to search more
        }
    }
    
    //If no tile was found, -1 is returned. Otherwise, the index pointing to the coordinate in the array of glm::vec2's is returned
    //Since there are double the number of coordinates, this coordinate times 2 is the first coordinate of the tile in vertexData
    
    //Return negative coordinates if the click is outside of all tiles
    if (tileIndex == -1)
        return NO_SELECTION;
    
    glm::ivec2 tileIndexVec;
    
    tileIndexVec.x = (int)(tileIndex / BOARD_WIDTH); //The x index in the 2D vector
    
    tileIndexVec.y = tileIndex - (BOARD_WIDTH * tileIndexVec.x); //The y index in the 2D vector
    
    return tileIndexVec;
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
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->gameBoard.get(tile.x(), tile.y() - 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard.get(tile.x() - 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard.height(tile.x()) - 1) {
                    if (this->gameBoard.get(tile.x(), tile.y() + 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //West
                if (tile.x() < this->gameBoard.width() - 1) {
                    if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
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

/*
 * TO ADD:
 *
 * CHECK IF THE OCCUPYING CREATURE ON REACHABLE SQUARES ARE ATTACKABLE SPECIFICALLY BY THE CREATURE.
 */
std::vector<Tile> Game::getAttackableTiles(Tile creatureTile) {
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, GLint> > reachedTiles; //This is a vector containing the tiles found so far, along with the energy the creature has at that tile
        
        std::vector<Tile> attackableTiles; //A vector of the tiles that can be attacked
        
        reachedTiles.push_back(std::pair<Tile, GLint>(creatureTile, creatureTile.creature()->range()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->gameBoard.get(tile.x(), tile.y() - 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() - 1).occupied()) {
                        attackableTiles.push_back(this->gameBoard.get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
                        std::cout << "added North tile\n";
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard.get(tile.x() - 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() - 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->gameBoard.get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
                        std::cout << "added East tile\n";
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard.height(tile.x()) - 1) {
                    if (this->gameBoard.get(tile.x(), tile.y() + 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() + 1).occupied()) {
                        attackableTiles.push_back(this->gameBoard.get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
                        std::cout << "added South tile\n";
                    }
                }
                
                //West
                if (tile.x() < this->gameBoard.width() - 1) {
                    if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() + 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->gameBoard.get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
                        std::cout << "added West tile\n";
                    }
                }
            }
        }
        
        return attackableTiles;
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