//
//  Host.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Host.hpp"

Host::Host(unsigned int numberOfPlayers, int portNum, Board gameBoard) : board(gameBoard), socket(ServerSocket()) {
    this->socket.setSocket(portNum);
    for (int a = 0; a < numberOfPlayers; a++) {
        this->socket.addClient();
    }
    
    //Send initial info to the visualizer saying the width and height of the board
    
    std::string initialData;
    initialData += std::to_string(this->board.width()) + ",";
    initialData += std::to_string(this->board.height(0)) + ",";
    
    this->socket.broadcast(initialData);
    if (!this->socket.allReceived("initialDataReceived"))
        throw std::runtime_error("Initial data not received");
    
    int playerNum = 0;
    while (this->players.size() < numberOfPlayers) {
        this->players.push_back(Player(&board, playerNum++));
    }
    
    this->programStartTime = std::chrono::steady_clock::now();
    this->lastFrame = std::chrono::steady_clock::now() - this->programStartTime;
    
    std::vector<int> terrainData, creatureData, damageData, buildingData;
    std::vector<std::vector<float> > colorDataVec(this->socket.numberOfClients()); //Vector of color data for each player
    std::vector<float> offsetData;
    
    this->getBufferData(&terrainData, &creatureData, &colorDataVec, &damageData, &offsetData, &buildingData);
    
    this->socket.broadcast(Host::storeVectorOfInts(terrainData));
    if (!this->socket.allReceived("terrainDataReceived"))
        throw std::runtime_error("Terrain data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(creatureData));
    if (!this->socket.allReceived("creatureDataReceived"))
        throw std::runtime_error("Creature data not received");
    
    for (int a = 0; a < this->socket.numberOfClients(); a++) {
        this->socket.send(Host::storeVectorOfFloats(colorDataVec[a]), a);
    }
    if (!this->socket.allReceived("colorDataReceived"))
        throw std::runtime_error("Color data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(damageData));
    if (!this->socket.allReceived("damageDataReceived"))
        throw std::runtime_error("Damage data not received");
    
    this->socket.broadcast(Host::storeVectorOfFloats(offsetData));
    if (!this->socket.allReceived("offsetDataReceived"))
        throw std::runtime_error("Offset data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(buildingData));
    if (!this->socket.allReceived("buildingDataReceived"))
        throw std::runtime_error("Building data not received");
}

std::string Host::Host::storeVectorOfInts(std::vector<int> vec) {
    std::string str;
    
    for (int a = 0; a < vec.size(); a++) {
        str += std::to_string(vec[a]) + ',';
    }
    
    return str;
}

std::string Host::Host::storeVectorOfFloats(std::vector<float> vec) {
    std::string str;
    
    for (int a = 0; a < vec.size(); a++) {
        str += std::to_string(vec[a]) + ',';
    }
    
    return str;
}

void Host::update() {
    //Update frame information first
    std::chrono::duration<float> currentFrame = std::chrono::steady_clock::now() - this->programStartTime;
    this->deltaTime = currentFrame.count() - this->lastFrame.count();
    
    //Go through all tiles' damage and reset them if enough time has passed
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            if (this->board.get(x, y).damage() > 0 && currentFrame.count() - this->board.get(x, y).timeOfDamage() > Tile::damageScreenTime)
                this->board.setDamage(x, y, 0, currentFrame.count());
        }
    }
    
    //Get info from the clients about mouse position
    std::vector<std::string> clientInfo;
    
    for (int a = 0; a < this->socket.numberOfClients(); a++) {
        clientInfo.push_back(this->socket.receive(a));
    }
    this->socket.broadcast("clientDataReceived");
    
    std::vector<int> terrainData, creatureData, damageData, buildingData;
    std::vector<std::vector<float> > colorDataVec(this->socket.numberOfClients()); //Vector of color data for each player
    std::vector<float> offsetData;
    
    this->getBufferData(&terrainData, &creatureData, &colorDataVec, &damageData, &offsetData, &buildingData);
    
    this->socket.broadcast(Host::storeVectorOfInts(terrainData));
    if (!this->socket.allReceived("terrainDataReceived"))
        throw std::runtime_error("Terrain data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(creatureData));
    if (!this->socket.allReceived("creatureDataReceived"))
        throw std::runtime_error("Creature data not received");
    
    for (int a = 0; a < this->socket.numberOfClients(); a++) {
        this->socket.send(Host::storeVectorOfFloats(colorDataVec[a]), a);
    }
    if (!this->socket.allReceived("colorDataReceived"))
        throw std::runtime_error("Color data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(damageData));
    if (!this->socket.allReceived("damageDataReceived"))
        throw std::runtime_error("Damage data not received");
    
    this->socket.broadcast(Host::storeVectorOfFloats(offsetData));
    if (!this->socket.allReceived("offsetDataReceived"))
        throw std::runtime_error("Offset data not received");
    
    this->socket.broadcast(Host::storeVectorOfInts(buildingData));
    if (!this->socket.allReceived("buildingDataReceived"))
        throw std::runtime_error("Building data not received");

    
    /* Each index of clientInfo stores information:
     1) Current mouse position's x coordinate on the board.
     2) Current mouse position's y coordinate on the board.
            -1 for either (1) or (2) represents NO_SELECTION
     3) Whether the mouse is down or not. '0' represents up (false), and '1' represents down (true).
     
     Any other input should be done here. That hasn't been implimented.
     */
    
    for (int a = 0; a < clientInfo.size(); a++) {
        glm::ivec2 selectedTile;
        
        selectedTile.x = std::stoi(clientInfo[a].substr(0, clientInfo[a].find_first_of(','))); //Convert the substring to an int
        
        clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        
        selectedTile.y = std::stoi(clientInfo[a].substr(0, clientInfo[a].find_first_of(','))); //Convert the substring to an int
        
        clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        
        if (selectedTile.x < 0 || selectedTile.y < 0) {
            selectedTile = NO_SELECTION;
        }
        
        bool mouseDown = (clientInfo[a][0] - 48 == 0 ? false : true); //The first character in the string should be whether the mouse is up or down because the two parts before it, the mouse's x and y locations, were extracted and removed
        
        //Now extract the actions sent from the client
        clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(';') + 1, std::string::npos); //Set the string to the contain only the list of actions, starting with the first action string
        
        while (clientInfo.size() > 1) { //If there is more than just the semicolon at the end of the last
            this->processAction(clientInfo[a].substr(clientInfo[a].find_first_of(';')), a); //Process the next action string
            clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(';') + 1, std::string::npos); //Delete the processed action
        }
        
        this->players[a].updateSelected(mouseDown, selectedTile, this->activePlayer, currentFrame.count());
        
        this->players[a].updateCreatures(this->deltaTime, this->activePlayer);
    }
    
    this->socket.broadcast("End of frame");
    
    //Set the tiem of the previous frame to currentTime
    this->lastFrame = currentFrame;
}

//Private member functions

void Host::processAction(std::string action, unsigned int playerNum) {
    
    //Process actions here
    
}

void Host::getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<std::vector<float> >* colorDataVec, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData) {
    
    if (colorDataVec->size() < this->socket.numberOfClients())
        throw std::invalid_argument("Argument colorDataVec too small");
    
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
            
            for (int a = 0; a < this->socket.numberOfClients(); a++) {
                glm::vec3 tileColor = this->players[a].tileColor(x, y);
                (*colorDataVec)[a].push_back(tileColor.x);
                (*colorDataVec)[a].push_back(tileColor.y);
                (*colorDataVec)[a].push_back(tileColor.z);
            }
            
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

void Host::incrementActivePlayer() {
    this->activePlayer++;
    
    //Make sure the active player isn't greater than the largest index of players (size - 1)
    if (this->activePlayer >= this->players.size()) {
        this->activePlayer = 0;
    }
}
