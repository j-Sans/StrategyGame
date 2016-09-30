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
//        try {
        this->sockets.push_back(ServerSocket());
        
        this->sockets.back().setSocket(portNum);
    
        //Send initial info to the visualizer saying the width and height of the board
    
        std::string initialData;
        initialData.push_back(this->board.width());
        initialData.push_back(this->board.height(0));
    
        this->sockets.back().send(initialData);
        if (this->sockets.back().receive() != "initialDataReceived")
            throw std::runtime_error("Initial data not received");
    
        portNum++;
//        } catch (std::exception e) {
//            std::cout << "Error initializing socket: " << e.what() << std::endl;
//            throw std::runtime_error("Host couldn't be set");
//        }
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
            if (this->board.get(x, y).creature() != nullptr) { //If there is a creature set the data properly, otherwise as 0
                creatureData.push_back(this->board.get(x, y).creature()->direction());
                creatureData.push_back(this->board.get(x, y).creature()->controller());
            } else {
                creatureData.push_back(0);
                creatureData.push_back(0);
            }
            
            glm::vec3 tileColor = this->players[0].game.tileColor(x, y);
            colorData.push_back(tileColor.x * 100);
            colorData.push_back(tileColor.y * 100);
            colorData.push_back(tileColor.z * 100);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                offsetData.push_back(this->board.get(x, y).creature()->offset() * 100);
            else
                offsetData.push_back(0);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                buildingData.push_back(this->board.get(x, y).building()->controller());
            else
                buildingData.push_back(0);
        }
    }
    
    this->sockets[0].send(terrainData);
    if (this->sockets[0].receive() != "terrainDataReceived")
        throw std::runtime_error("Terrain data not received");
    
    this->sockets[0].send(creatureData);
    if (this->sockets[0].receive() != "creatureDataReceived")
        throw std::runtime_error("Creature data not received");
    
    this->sockets[0].send(colorData);
    if (this->sockets[0].receive() != "colorDataReceived")
        throw std::runtime_error("Color data not received");
    
    this->sockets[0].send(damageData);
    if (this->sockets[0].receive() != "damageDataReceived")
        throw std::runtime_error("Damage data not received");
    
    this->sockets[0].send(offsetData);
    if (this->sockets[0].receive() != "offsetDataReceived")
        throw std::runtime_error("Offset data not received");
    
    this->sockets[0].send(buildingData);
    if (this->sockets[0].receive() != "buildingDataReceived")
        throw std::runtime_error("Building data not received");
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
            if (this->board.get(x, y).creature() != nullptr) { //If there is a creature set the data properly, otherwise as 0
                creatureData.push_back(this->board.get(x, y).creature()->direction());
                creatureData.push_back(this->board.get(x, y).creature()->controller());
            } else {
                creatureData.push_back(0);
                creatureData.push_back(0);
            }
            
            glm::vec3 tileColor = this->players[0].game.tileColor(x, y);
            colorData.push_back(tileColor.x * 100);
            colorData.push_back(tileColor.y * 100);
            colorData.push_back(tileColor.z * 100);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                offsetData.push_back(this->board.get(x, y).creature()->offset() * 100);
            else
                offsetData.push_back(0);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                buildingData.push_back(this->board.get(x, y).building()->controller());
            else
                buildingData.push_back(0);
        }
    }
    
    this->sockets[0].send(terrainData);
    if (this->sockets[0].receive() != "terrainDataReceived")
        throw std::runtime_error("Terrain data not received");

    this->sockets[0].send(creatureData);
    if (this->sockets[0].receive() != "creatureDataReceived")
        throw std::runtime_error("Creature data not received");

    this->sockets[0].send(colorData);
    if (this->sockets[0].receive() != "colorDataReceived")
        throw std::runtime_error("Color data not received");

    this->sockets[0].send(damageData);
    if (this->sockets[0].receive() != "damageDataReceived")
        throw std::runtime_error("Damage data not received");

    this->sockets[0].send(offsetData);
    if (this->sockets[0].receive() != "offsetDataReceived")
        throw std::runtime_error("Offset data not received");

    this->sockets[0].send(buildingData);
    if (this->sockets[0].receive() != "buildingDataReceived")
        throw std::runtime_error("Building data not received");

    
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
