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
    
    /*!
     * The host of the game, containing the players and the board.
     *
     * @param numberOfPlayers The number of players with which to initialize the game.
     * @param portNum The port on this system (the server side) where to start taking connections. Succeeding ports may also be used, depending on implementation and number of players.
     * @param gameBoard A board to be used as the game board. Passed by value, so the game board will actually be stored within the host class.
     */
    Host(unsigned int numberOfPlayers, int portNum, Board gameBoard);
    
    //Public member functions
    
    static std::string storeVectorOfInts(std::vector<int> vec);
    
    static std::string storeVectorOfFloats(std::vector<float> vec);
    
    /*!
     * Updates the players and the game based on input from clients. This should be called every frame.
     */
    void update();
    
private:
    //Private properties
    
    //Time
    float programStartTime = clock() / CLOCKS_PER_SEC;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    //Turn
    unsigned int activePlayer = 0;
    
    //Clients
    std::vector<ServerSocket> sockets;
    
    //Game
    std::vector<Player> players;
    Board board;
    
    //Private member functions
    
    /*!
     * Changes the active player to the next one in the turn cycle.
     */
    void incrementActivePlayer();
    
};

#endif /* Host_hpp */
