//
//  Client.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Client.hpp"

Client::Client(std::string hostName, int portNum) : visualizer(Visualizer("Shaders/board/board.vert", "Shaders/board/board.geom", "Shaders/board/board.frag")), board(Board(std::vector<std::vector<Tile> >(0))) {
    
    this->socket.setSocket(hostName, portNum);
    
    std::string initialInfo = this->socket.receive();
    this->socket.send("initialDataReceived");
    
    //Read in the board size
    int boardWidth = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    int boardHeight = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    this->playerNum = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(',')));
    
    this->board = Board::deserialize(this->socket.receive());
    this->socket.send("boardReceived");
    
    for (int x = 0; x < this->board.width(); x++) {
        std::vector<std::array<int, 2> > boardInfoColumn(this->board.height(x));
        std::vector<std::queue<std::string> > tileActionColumn(this->board.height(x));
        this->boardInfo.push_back(boardInfoColumn);
        this->tileActions.push_back(tileActionColumn);
    }
    
    this->visualizer.set(boardWidth, boardHeight);
    
    this->getBufferData(&this->visualizer.terrainData, &this->visualizer.creatureData, &this->visualizer.colorData, &this->visualizer.damageData, &this->visualizer.offsetData, &this->visualizer.buildingData);
    
    
//    std::vector<int> terrainDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("terrainDataReceived");
//    
//    std::vector<int> creatureDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("creatureDataReceived");
//    
//    std::vector<float> colorDataVec = Client::parseVectorOfFloat(this->socket.receive());
//    this->socket.send("colorDataReceived");
//    
//    std::vector<int> damageDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("damageDataReceived");
//    
//    std::vector<float> offsetDataVec = Client::parseVectorOfFloat(this->socket.receive());
//    this->socket.send("offsetDataReceived");
//    
//    std::vector<int> buildingDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("buildingDataReceived");
//    
//    this->visualizer.set(boardWidth, boardHeight, terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
}

void Client::render() {
    this->visualizer.startFrame();
    
    this->updateSelected(this->visualizer.mousePressed(), this->visualizer.getMouseTile(), glfwGetTime());
    
    std::string clientInfo = this->visualizer.getClientInfo() + ";";
    
    for (auto a = this->actionsForClientInfo.begin(); a != this->actionsForClientInfo.end(); a++) {
        clientInfo += *a + ";";
        a = this->actionsForClientInfo.erase(a);
    }
    
    this->getBufferData(&this->visualizer.terrainData, &this->visualizer.creatureData, &this->visualizer.colorData, &this->visualizer.damageData, &this->visualizer.offsetData, &this->visualizer.buildingData);
    
    this->visualizer.render(/*terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec*/);
    
    this->visualizer.endFrame();
    
//    this->socket.send(clientInfo);
//    if (this->socket.receive() != "clientDataReceived")
//        throw std::runtime_error("Client data not received");
//    
//    this->visualizer.activePlayer = std::stoi(this->socket.receive());
//    this->socket.send("activePlayerReceived");
//    
//    std::vector<int> terrainDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("terrainDataReceived");
//    
//    std::vector<int> creatureDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("creatureDataReceived");
//    
//    std::vector<float> colorDataVec = Client::parseVectorOfFloat(this->socket.receive());
//    this->socket.send("colorDataReceived");
//    
//    std::vector<int> damageDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("damageDataReceived");
//    
//    std::vector<float> offsetDataVec = Client::parseVectorOfFloat(this->socket.receive());
//    this->socket.send("offsetDataReceived");
//    
//    std::vector<int> buildingDataVec = Client::parseVectorOfInt(this->socket.receive());
//    this->socket.send("buildingDataReceived");
//    
//    if (this->socket.receive() != "End of frame")
//        throw std::runtime_error("Waiting on host to move to next frame");
}

void Client::terminate() {
    this->visualizer.terminate();
}

bool Client::windowShouldClose() {
    return this->visualizer.window.shouldClose();
}

void Client::resetAllTiles() {
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            //Set all tile styles to regular
            this->boardInfo[x][y][TILE_STYLE] = REGULAR;
            
            //Empty all queues of action for each tile
            while (this->tileActions[x][y].size() > 0)
                this->tileActions[x][y].pop();
        }
    }
}

void Client::updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int currentTime) {
    //Update all tiles other than the one where the mouse is to have no hovering
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            this->boardInfo[x][y][TILE_HOVER] = NO_HOVERING;
        }
    }
    
    if (this->validTile(mousePos)) {
        this->boardInfo[mousePos.x][mousePos.y][TILE_HOVER] = HOVERING;
    }
    
    if (mouseDown) {
        if (mousePos == NO_SELECTION) {
            this->resetAllTiles();
        } else if (mousePos == INTERFACE_BOX_SELECTION) {
        } else if (mousePos == this->selectedTile) { //Reset the tile (and others) if the current tile is clicked again
            this->resetAllTiles();
            
            //Set selectedTile to null results
            this->selectedTile = NO_SELECTION;
        } else {
            switch (this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE]) {
                    
                case REGULAR: { //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
                    this->resetAllTiles();
                    
                    //Select this new tile
                    this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] = SELECTED;
                    
                    Creature* creature = this->board.get(mousePos.x, mousePos.y).creature();
                    
                    //If the selected tile is a creature, highlight reachable tiles and update the creature's direction
                    if (creature != nullptr && creature->controller() == this->playerNum) {
                        std::vector<Tile> reachableTiles = getReachableTiles(this->board.get(mousePos.x, mousePos.y));
                        
                        for (int a = 0; a < reachableTiles.size(); a++) {
                            if (this->board.get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(*creature)) {
                                this->boardInfo[reachableTiles[a].x()][reachableTiles[a].y()][TILE_STYLE] = REACHABLE;
                                this->tileActions[reachableTiles[a].x()][reachableTiles[a].y()].push("move_creature_at_" + std::to_string(reachableTiles[a].x()) + "_" + std::to_string(reachableTiles[a].y()));
                            }
                        }
                        
                        if (creature->energy() > 0) {
                            std::vector<Tile> attackableTiles = getAttackableTiles(this->board.get(mousePos.x, mousePos.y));
                            
                            for (int a = 0; a < attackableTiles.size(); a++) {
                                //If there is a creature or building on the tile, controlled by an opponent, make it attackable
                                if ((this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).creature() != nullptr && this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).creature()->controller() != this->playerNum) || (this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).building() != nullptr && this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).building()->controller() != this->playerNum)) {
                                    this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
                                    this->tileActions[reachableTiles[a].x()][reachableTiles[a].y()].push("attack_from_" + std::to_string(reachableTiles[a].x()) + "_" + std::to_string(reachableTiles[a].y()));
                                }
                            }
                        }
                    }
                    this->selectedTile = mousePos;
                    break;
                } case REACHABLE: {
                    this->resolveTileAction(mousePos.x, mousePos.y);
                    break;
                } case ATTACKABLE: {
                    /*
                     glm::ivec2 attacker = glm::ivec2(this->selectedTile.x, this->selectedTile.y);
                    glm::ivec2 defender = glm::ivec2(mousePos.x, mousePos.y);
                    
                    if (this->board->tileDistances(attacker.x, attacker.y, defender.x, defender.y) <= this->board->get(attacker.x, attacker.y).creature()->range()) {
                        
                        int attackDamage = 0, defendDamage = 0;
                        
                        this->board->initiateCombat(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
                        this->board->setDamage(defender.x, defender.y, attackDamage, currentTime); //Make the damage visible
                        this->board->setDamage(attacker.x, attacker.y, defendDamage, currentTime); //For attacker and defender
                        
                        this->resetAllTiles();
                        this->selectedTile = NO_SELECTION;
                        
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                        //If the attacker died, nothing will happen and the function will return false
                        this->selectCreature(attacker.x, attacker.y);
#endif
                    }
                    break;
                     */
                    this->resolveTileAction(mousePos.x, mousePos.y);
                }
            }
        }
    }
}

std::vector<Tile> Client::getReachableTiles(Tile creatureTile) {
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
                    if (this->board.get(tile.x(), tile.y() - 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() - 1))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->board.get(tile.x() - 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() - 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //South
                if (tile.y() < this->board.height(tile.x()) - 1) {
                    if (this->board.get(tile.x(), tile.y() + 1).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() + 1))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    }
                }
                
                //West
                if (tile.x() < this->board.width() - 1) {
                    if (this->board.get(tile.x() + 1, tile.y()).passableByCreature(creature) && reachedTiles[tileIterator].second >= this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() + 1, tile.y()))) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainMovementCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
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


std::vector<Tile> Client::getAttackableTiles(Tile creatureTile) {
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
                    if (this->board.get(tile.x(), tile.y() - 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() - 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->board.get(tile.x(), tile.y() - 1).occupied()) {
                        attackableTiles.push_back(this->board.get(tile.x(), tile.y() - 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->board.get(tile.x() - 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() - 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->board.get(tile.x() - 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->board.get(tile.x() - 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //South
                if (tile.y() < this->board.height(tile.x()) - 1) {
                    if (this->board.get(tile.x(), tile.y() + 1).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x(), tile.y() + 1)))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->board.get(tile.x(), tile.y() + 1).occupied()) {
                        attackableTiles.push_back(this->board.get(tile.x(), tile.y() + 1)); //Add the found tile to the vector of attackable tiles
                    }
                }
                
                //West
                if (tile.x() < this->board.width() - 1) {
                    if (this->board.get(tile.x() + 1, tile.y()).passableByAttackStyle(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->board.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - this->board.getTerrainAttackCost(this->board.get(tile.x(), tile.y()), this->board.get(tile.x() + 1, tile.y())))); //Add the found tile to the reached tiles, along with the remaining range the creature would have - 1.
                    }
                    if (this->board.get(tile.x() + 1, tile.y()).occupied()) {
                        attackableTiles.push_back(this->board.get(tile.x() + 1, tile.y())); //Add the found tile to the vector of attackable tiles
                    }
                }
            }
        }
        
        return attackableTiles;
    }
}

void Client::getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<float>* colorDataVec, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData) {
    
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            (*terrainData).push_back(this->board.get(x, y).terrain());
            
            (*creatureData).push_back(this->board.get(x, y).creatureType());
            if (this->board.get(x, y).creature() != nullptr) { //If there is a creature set the data properly, otherwise as 0
                (*creatureData).push_back(this->board.get(x, y).creature()->direction());
                (*creatureData).push_back(this->board.get(x, y).creature()->controller());
            } else {
                (*creatureData).push_back(0);
                (*creatureData).push_back(0);
            }
            
            glm::vec3 tileColor = this->tileColor(x, y);
            (*colorDataVec).push_back(tileColor.x);
            (*colorDataVec).push_back(tileColor.y);
            (*colorDataVec).push_back(tileColor.z);
            
            (*damageData).push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                (*offsetData).push_back(this->board.get(x, y).creature()->offset());
            else
                (*offsetData).push_back(0);
            
            (*buildingData).push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                (*buildingData).push_back(this->board.get(x, y).building()->controller());
            else
                (*buildingData).push_back(0);
        }
    }
}

glm::vec3 Client::tileColor(unsigned int x, unsigned int y) {
    if (x >= this->board.width())
        throw std::range_error("X out of range");
    if (y >= this->board.height(x))
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

void Client::resolveTileAction(int x, int y) {
    if (x > this->tileActions.size())
        throw std::range_error("X out of bounds");
    if (this->tileActions.size() < 1)
        throw std::logic_error("No board size");
    if (y > this->tileActions[x].size())
        throw std::range_error("Y out of bounds");
    if (this->tileActions[x][y].size() < 1)
        throw std::logic_error("No actions to call");
    
    std::string action = this->tileActions[x][y].front();
    this->tileActions[x][y].pop();
    
    this->actionsForClientInfo.push_back(std::to_string(x) + "," + std::to_string(y) + "," + action);
}

bool Client::validTile(glm::ivec2 tilePos) {
    if (tilePos.x >= 0 && tilePos.x < this->board.width() && tilePos.y >= 0 && tilePos.y < this->board.height(tilePos.x))
        return true;
    else
        return false;
}
