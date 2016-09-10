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
        }
    }
    
    while (this->players.size() < numberOfPlayers) {
        this->players.push_back(Player(&board));
    }
}

void Host::update(bool mouseDown, glm::vec2 selectedTile) {
    GLfloat currentFrame = (clock() / CLOCKS_PER_SEC) - this->programStartTime;
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
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