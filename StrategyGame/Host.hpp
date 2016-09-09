//
//  Host.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Host_hpp
#define Host_hpp

//Standard library includes
#include <time.h>
#include <vector>
#include <exception>

//Local includes
#include "ServerSocket.hpp"
#include "Player.hpp"
#include "Board.hpp"

class Host {
public:
    
    //Constructor
    Host(unsigned int numberOfPlayers, int portNum, Board gameBoard);
    
    void update();
    
private:
    //Private properties
    
    float programStartTime = clock() / CLOCKS_PER_SEC;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    unsigned int activePlayer = 0;
    
    std::vector<ServerSocket> sockets;
    
    std::vector<Player> players;
    
    Board board;
    
    //Private member functions
    
    /*!
     * Changes the active player to the next one in the turn cycle.
     */
    void incrementActivePlayer();
    
};

#endif /* Host_hpp */
