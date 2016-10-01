//
//  Client.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Client.hpp"

Client::Client(std::string hostName, int portNum) : visualizer(Visualizer("Shaders/board/board.vert", "Shaders/board/board.geom", "Shaders/board/board.frag")) {
    std::map<BoardInfoDataTypes, std::string> boardInfo;
    
    this->socket.setSocket(hostName, portNum);
    
    std::string initialInfo = this->socket.receive();
    this->socket.send("initialDataReceived");
    
    //std::stoi converts from string to int
    
    unsigned int boardWidth = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    unsigned int boardHeight = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    
    boardInfo[TerrainData] = this->socket.receive();
    this->socket.send("terrainDataReceived");
    
    boardInfo[CreatureData] = this->socket.receive();
    this->socket.send("creatureDataReceived");
    
    boardInfo[ColorData] = this->socket.receive();
    this->socket.send("colorDataReceived");
    
    boardInfo[DamageData] = this->socket.receive();
    this->socket.send("damageDataReceived");
    
    boardInfo[OffsetData] = this->socket.receive();
    this->socket.send("offsetDataReceived");
    
    boardInfo[BuildingData] = this->socket.receive();
    this->socket.send("buildingDataReceived");
    
    std::cout << "Socket communication finished" << std::endl;
    
    this->visualizer.set(boardWidth, boardHeight, boardInfo);
}

void Client::render(std::map<BoardInfoDataTypes, std::string> boardInfo) {
    this->socket.send(this->visualizer.getClientInfo());

    boardInfo[TerrainData] = this->socket.receive();
    this->socket.send("terrainDataReceived");

    boardInfo[CreatureData] = this->socket.receive();
    this->socket.send("creatureDataReceived");

    boardInfo[ColorData] = this->socket.receive();
    this->socket.send("colorDataReceived");

    boardInfo[DamageData] = this->socket.receive();
    this->socket.send("damageDataReceived");

    boardInfo[OffsetData] = this->socket.receive();
    this->socket.send("offsetDataReceived");

    boardInfo[BuildingData] = this->socket.receive();
    this->socket.send("buildingDataReceived");
    
    this->visualizer.render(boardInfo);
}

std::vector<int> parseVectorOfInt(std::string str) {
    std::vector<int> vec;
    
    while (str.length() > 0) {
        int nextCommaPos = (int)str.find_first_of(',');
        
        //If there are no more commas, the last element has been found
        if (nextCommaPos == std::string::npos) {
            vec.push_back(std::stoi(str));
            break;
        } else {
            vec.push_back(std::stoi(str.substr(0, nextCommaPos))); //Convert the substring to an int and add it to the vector
            str = str.substr(nextCommaPos + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        }
    }
    
    return vec;
}

std::vector<float> parseVectorOfDouble(std::string str) {
    std::vector<float> vec;
    
    while (str.length() > 0) {
        int nextCommaPos = (int)str.find_first_of(',');
        
        //If there are no more commas, the last element has been found
        if (nextCommaPos == std::string::npos) {
            vec.push_back(std::stof(str));
            break;
        } else {
            vec.push_back(std::stof(str.substr(0, nextCommaPos))); //Convert the substring to an int and add it to the vectors
            str = str.substr(nextCommaPos + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        }
    }
    
    return vec;
}
