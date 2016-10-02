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
    
    //std::stoi converts from string to int
    
    unsigned int boardWidth = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    unsigned int boardHeight = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(','))); //Convert the substring to an int
    
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
    
    std::cout << "Socket communication finished" << std::endl;
    
    this->visualizer.set(boardWidth, boardHeight, terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
}

std::vector<int> Client::parseVectorOfInt(std::string str) {
    std::vector<int> vec;
    
    while (str.length() > 0) {
        int nextCommaPos = (int)str.find_first_of(',');
        
        //If there are no more commas or a comma is at the end, the last element has been found
        if (nextCommaPos == std::string::npos || nextCommaPos == str.length() - 1) {
            vec.push_back(std::stoi(str));
            break;
        } else {
            vec.push_back(std::stoi(str.substr(0, nextCommaPos))); //Convert the substring to an int and add it to the vector
            str = str.substr(nextCommaPos + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        }
    }
    
    return vec;
}

std::vector<float> Client::parseVectorOfFloat(std::string str) {
    std::vector<float> vec;
    
    while (str.length() > 0) {
        int nextCommaPos = (int)str.find_first_of(',');
        
        //If there are no more commas or a comma is at the end, the last element has been found
        if (nextCommaPos == std::string::npos || nextCommaPos == str.length() - 1) {
            vec.push_back(std::stof(str));
            break;
        } else {
            vec.push_back(std::stof(str.substr(0, nextCommaPos))); //Convert the substring to an int and add it to the vectors
            str = str.substr(nextCommaPos + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        }
    }
    
    return vec;
}

void Client::render() {
    this->socket.send(this->visualizer.getClientInfo());
    if (this->socket.receive() != "clientDataReceived")
        throw std::runtime_error("Client data not received");

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
    
    this->visualizer.render(terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
}

void Client::terminate() {
    this->visualizer.terminate();
}

bool Client::windowShouldClose() {
    return glfwWindowShouldClose(this->visualizer.window());
}
