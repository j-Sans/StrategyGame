//
//  Client.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Client.hpp"

Client::Client(std::string hostName, int portNum) {
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
    
    Visualizer
}



this->socket.send(clientInfo);

std::map<BoardInfoDataTypes, std::string> boardInfo;

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
