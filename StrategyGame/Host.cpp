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
        initialData += std::to_string(this->board.width()) + ",";
        initialData += std::to_string(this->board.height(0)) + ",";
    
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
    
    std::vector<int> terrainData;
    std::vector<int> creatureData;
    std::vector<float> colorData;
    std::vector<int> damageData;
    std::vector<float> offsetData;
    std::vector<int> buildingData;
    
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
            colorData.push_back(tileColor.x);
            colorData.push_back(tileColor.y);
            colorData.push_back(tileColor.z);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                offsetData.push_back(this->board.get(x, y).creature()->offset());
            else
                offsetData.push_back(0);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                buildingData.push_back(this->board.get(x, y).building()->controller());
            else
                buildingData.push_back(0);
        }
    }
    
    this->sockets[0].send(Host::storeVectorOfInts(terrainData));
    if (this->sockets[0].receive() != "terrainDataReceived")
        throw std::runtime_error("Terrain data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(creatureData));
    if (this->sockets[0].receive() != "creatureDataReceived")
        throw std::runtime_error("Creature data not received");
    
    this->sockets[0].send(Host::storeVectorOfFloats(colorData));
    if (this->sockets[0].receive() != "colorDataReceived")
        throw std::runtime_error("Color data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(damageData));
    if (this->sockets[0].receive() != "damageDataReceived")
        throw std::runtime_error("Damage data not received");
    
    this->sockets[0].send(Host::storeVectorOfFloats(offsetData));
    if (this->sockets[0].receive() != "offsetDataReceived")
        throw std::runtime_error("Offset data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(buildingData));
    if (this->sockets[0].receive() != "buildingDataReceived")
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
    std::cout << "Next frame for host" << std::endl << std::endl;
    
    //Update frame information first
    GLfloat currentFrame = (clock() / CLOCKS_PER_SEC) - this->programStartTime;
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
    std::string clientInfo = this->sockets[0].receive();
    this->sockets[0].send("clientDataReceived");
    
    std::vector<int> terrainData;
    std::vector<int> creatureData;
    std::vector<float> colorData;
    std::vector<int> damageData;
    std::vector<float> offsetData;
    std::vector<int> buildingData;
    
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
            colorData.push_back(tileColor.x);
            colorData.push_back(tileColor.y);
            colorData.push_back(tileColor.z);
            
            damageData.push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                offsetData.push_back(this->board.get(x, y).creature()->offset());
            else
                offsetData.push_back(0);
            
            buildingData.push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                buildingData.push_back(this->board.get(x, y).building()->controller());
            else
                buildingData.push_back(0);
        }
    }
    
    this->sockets[0].send(Host::storeVectorOfInts(terrainData));
    if (this->sockets[0].receive() != "terrainDataReceived")
        throw std::runtime_error("Terrain data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(creatureData));
    if (this->sockets[0].receive() != "creatureDataReceived")
        throw std::runtime_error("Creature data not received");
    
    this->sockets[0].send(Host::storeVectorOfFloats(colorData));
    if (this->sockets[0].receive() != "colorDataReceived")
        throw std::runtime_error("Color data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(damageData));
    if (this->sockets[0].receive() != "damageDataReceived")
        throw std::runtime_error("Damage data not received");
    
    this->sockets[0].send(Host::storeVectorOfFloats(offsetData));
    if (this->sockets[0].receive() != "offsetDataReceived")
        throw std::runtime_error("Offset data not received");
    
    this->sockets[0].send(Host::storeVectorOfInts(buildingData));
    if (this->sockets[0].receive() != "buildingDataReceived")
        throw std::runtime_error("Building data not received");

    
    /* clientInfo stores information:
     1) Current mouse position's x coordinate on the board.
     2) Current mouse position's y coordinate on the board.
            -1 for either (1) or (2) represents NO_SELECTION
     3) Whether the mouse is down or not. '0' represents up (false), and '1' represents down (true).
     
     Any other input should be done here. That hasn't been implimented.
     */
    
    std::cout << "Error point 1" << std::endl;
    
    glm::ivec2 selectedTile;
    
    std::cout << "Client info: " << clientInfo << std::endl;
    
    selectedTile.x = std::stoi(clientInfo.substr(0, clientInfo.find_first_of(','))); //Convert the substring to an int
    
    clientInfo = clientInfo.substr(clientInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    std::cout << "Error point 2" << std::endl;
    
    std::cout << "Client info: " << clientInfo << std::endl;
    
    selectedTile.y = std::stoi(clientInfo.substr(0, clientInfo.find_first_of(','))); //Convert the substring to an int
    
    clientInfo = clientInfo.substr(clientInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    std::cout << "selectedTile: (" << selectedTile.x << ", " << selectedTile.y << ")" << std::endl;
    
    if (selectedTile.x == -1 || selectedTile.y == -1) {
        selectedTile = NO_SELECTION;
    }
    
    std::cout << "Error point 3" << std::endl;
    
    std::cout << "Client info mouse down bool (1 is true, 0 is false): " << clientInfo << std::endl;
    
    bool mouseDown = (clientInfo[0] - 48 == 0 ? false : true); //The first character in the string should be whether the mouse is up or down because the two parts before it, the mouse's x and y locations, were extracted and removed
    
    std::cout << "Error point 4" << std::endl;
    
    this->players[this->activePlayer].game.updateSelected(mouseDown, selectedTile, this->activePlayer);
    
    std::cout << "Error point 5" << std::endl;
    
    this->players[this->activePlayer].game.updateCreatures(this->deltaTime, this->activePlayer);
    
    std::cout << "End of frame for host" << std::endl;
    
    this->sockets[0].send("End of frame");
}

void Host::incrementActivePlayer() {
    this->activePlayer++;
    
    //Make sure the active player isn't greater than the largest index of players (size - 1)
    if (this->activePlayer >= this->players.size()) {
        this->activePlayer = 0;
    }
}
