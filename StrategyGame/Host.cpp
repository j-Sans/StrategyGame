//
//  Host.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Host.hpp"

Host::Host(unsigned int numberOfPlayers, int portNum) {
    try {
        this->socket.setSocket(portNum);
    } catch (std::exception e) {
        std::cout << "Error initializing socket" << std::endl;
        
        //CAN CONSTRUCTORS THROW ERRORS? IS IT BAD PRACTICE?
    }
    
    while (this->players.size() < numberOfPlayers) {
        this->players.push_back(Player());
    }
}