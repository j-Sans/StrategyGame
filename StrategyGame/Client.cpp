//
//  Client.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Client.hpp"

Client::Client(std::string hostName, int portNum) : visualizer(Visualizer("Shaders/board/board.vert", "Shaders/board/board.geom", "Shaders/board/board.frag")) {
    
    this->socket.setSocket(hostName, portNum);
    
    std::string initialInfo = this->socket.receive();
    this->socket.send("initialDataReceived");
    
    //Read in the board size
    this->boardWidth = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    this->boardHeight = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    this->playerNum = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(',')));
    
    //Initialize the vector of offsets to all have a value of 0
    this->offsets = std::vector<std::vector<float> >(this->boardWidth);
    for (int x = 0; x < this->boardWidth; x++) {
        for (int y = 0; y < this->boardHeight; y++) {
            this->offsets[x].push_back(0.0);
        }
    }
    
    
    
    
    
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
    
    this->socket.send(this->visualizer.getClientInfo());
//    if (this->socket.receive() != "clientDataReceived")
//        throw std::runtime_error("Client data not received");
    
    //this->visualizer.activePlayer = std::stoi(this->socket.receive());
    //this->socket.send("activePlayerReceived");
    
    std::vector<int> terrainDataVec = Client::parseVectorOfInt(this->socket.receive());
    this->socket.send("terrainDataReceived");
    
    std::vector<int> creatureDataVec = Client::parseVectorOfInt(this->socket.receive());
    this->socket.send("creatureDataReceived");
    
    std::vector<float> colorDataVec = Client::parseVectorOfFloat(this->socket.receive());
    this->socket.send("colorDataReceived");
    
    std::vector<int> damageDataVec = Client::parseVectorOfInt(this->socket.receive());
    this->socket.send("damageDataReceived");
    
    std::vector<float> offsetDataVec = Client::parseVectorOfFloat(this->socket.receive());
    this->socket.send("offsetDataReceived");
    
    std::vector<int> buildingDataVec = Client::parseVectorOfInt(this->socket.receive());
    this->socket.send("buildingDataReceived");
    
    if (this->socket.receive() != "End of frame")
        throw std::runtime_error("Waiting on host to move to next frame");
    
    this->visualizer.render(terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
    
    this->visualizer.endFrame();
}

void Client::terminate() {
    this->visualizer.terminate();
}

bool Client::windowShouldClose() {
    return glfwWindowShouldClose(this->visualizer.window());
}

void Client::updateCreatures(float deltaTime) {
    for (int x = 0; x < this->board->width(); x++) {
        for (int y = 0; y < this->board->height(x); y++) {
            
            Creature* creature = this->board->get(x, y).creature();
            
            if (creature != nullptr) {
                
                glm::ivec2 creatureLoc = glm::ivec2(x, y);
                
                unsigned int direction = creature->direction();
                
                if (direction == NORTH || direction == EAST) {
                    //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
                    
                    //If the creature is in the process of moving currently, continue to move it
                    if (creature->incrementOffset(deltaTime)) {
                        if (direction == NORTH) {
                            if (this->board->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.y -= 1;
                                
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                                if (creature->controller() == this->playerNum)
                                    this->selectCreature(x, y - 1);
#endif
                            }
                        } else if (direction == EAST) {
                            if (this->board->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                                creatureLoc.x -= 1;
                                
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                                if (creature->controller() == this->playerNum)
                                    this->selectCreature(x - 1, y);
#endif
                            }
                        }
                        
                    }
                } else if (direction == SOUTH || direction == WEST) {
                    
                    if (creature->incrementOffset(deltaTime)) {
                        
#ifndef RESET_SELECTED_TILE_AFTER_MOVEMENT
                        if (creature->controller() == this->playerNum)
                            this->selectCreature(x, y);
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
