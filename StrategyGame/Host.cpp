//
//  Host.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Host.hpp"

Host::Host(unsigned int numberOfPlayers, int portNum, Board gameBoard) : board(gameBoard) {
    while (this->sockets.size() < numberOfPlayers) {
        try {
            this->sockets.push_back(ServerSocket());
            
            this->sockets.back().setSocket(portNum);
            
            portNum++;
        } catch (std::exception e) {
            std::cout << "Error initializing socket: " << e.what() << std::endl;
            throw std::runtime_error("Host couldn't be set");
        }
    }
    
    while (this->players.size() < numberOfPlayers) {
        this->players.push_back(Player(&board));
    }
    
    std::string terrainData;
    std::string creatureData;
    std::string colorData;
    std::string damageData;
    std::string offsetData;
    std::string buildingData;
    
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            terrainData.push_back(this->board.get(x, y).terrain());
            
            creatureData.push_back(this->board.get(x, y).creatureType());
            creatureData.push_back(this->board.get(x, y).creature()->direction());
            creatureData.push_back(this->board.get(x, y).creature()->controller());
            
            glm::vec3 tileColor = this->players[0].game.tileColor(x, y);
            colorData.push_back(tileColor.x * 100);
            colorData.push_back(tileColor.y * 100);
            colorData.push_back(tileColor.z * 100);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            offsetData.push_back(this->board.get(x, y).creature()->offset() * 100);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            buildingData.push_back(this->board.get(x, y).building()->controller());
        }
    }
    
    this->sockets[0].send(terrainData);
    this->sockets[0].send(creatureData);
    this->sockets[0].send(colorData);
    this->sockets[0].send(damageData);
    this->sockets[0].send(offsetData);
    this->sockets[0].send(buildingData);
}

void Host::update() {
    //Update frame information first
    GLfloat currentFrame = (clock() / CLOCKS_PER_SEC) - this->programStartTime;
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
    std::string clientInfo = this->sockets[0].receive();
    
    std::string terrainData;
    std::string creatureData;
    std::string colorData;
    std::string damageData;
    std::string offsetData;
    std::string buildingData;
    
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            terrainData.push_back(this->board.get(x, y).terrain());
            
            creatureData.push_back(this->board.get(x, y).creatureType());
            creatureData.push_back(this->board.get(x, y).creature()->direction());
            creatureData.push_back(this->board.get(x, y).creature()->controller());
            
            glm::vec3 tileColor = this->players[0].game.tileColor(x, y);
            colorData.push_back(tileColor.x * 100);
            colorData.push_back(tileColor.y * 100);
            colorData.push_back(tileColor.z * 100);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            offsetData.push_back(this->board.get(x, y).creature()->offset() * 100);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            buildingData.push_back(this->board.get(x, y).building()->controller());
        }
    }
    
    this->sockets[0].send(terrainData);
    this->sockets[0].send(creatureData);
    this->sockets[0].send(colorData);
    this->sockets[0].send(damageData);
    this->sockets[0].send(offsetData);
    this->sockets[0].send(buildingData);
    
    /* clientInfo stores information:
     [0]: Current mouse position's x coordinate on the board.
     [1]: Current mouse position's y coordinate on the board.
            -1 for either [0] or [1] represents that no tile is being selected.
     [2]: Whether the mouse is down or not. 0 represents up (false), and 1 represents down (true). Everything else converts to down.
     
     Any other input should be done here. That hasn't been implimented.
     */
    
    glm::ivec2 selectedTile = glm::ivec2(clientInfo[0], clientInfo[1]);
    
    if (clientInfo[0] == -1 || clientInfo[1] == -1) {
        selectedTile = NO_SELECTION;
    }
    
    bool mouseDown = clientInfo[2] == 0 ? false : true;
    
    this->players[this->activePlayer].game.updateSelected(mouseDown, selectedTile, this->activePlayer);
    this->players[this->activePlayer].game.updateCreatures(this->deltaTime, this->activePlayer);
}

void Host::incrementActivePlayer() {
    this->activePlayer++;
    
    //Make sure the active player isn't greater than the largest index of players (size - 1)
    if (this->activePlayer >= this->players.size()) {
        this->activePlayer = 0;
    }
}