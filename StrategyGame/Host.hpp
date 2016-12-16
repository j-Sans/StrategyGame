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
#include <chrono>
#include <vector>
#include <list>
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
    
    //Public properties
    Board board;
    
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
    std::chrono::time_point<std::chrono::steady_clock> programStartTime;
    std::chrono::duration<float> lastFrame;
    float deltaTime = 0.0f;
    
    //Clients
    ServerSocket socket;
    
    //Game
    std::vector<Player> players;
    
    //Private member functions
    
    /*!
     * Follow an action based on a std::string sent from the client. The string comes from the clientInfo string.
     *
     * @param action The string telling what action to do.
     * @param playerNum The number of the player who sent the action.
     */
    void processAction(std::string action, unsigned int playerNum);
    
    /*!
     * Fill given std::vectors with OpenGL data.
     *
     * @param terrainData A pointer to an empty std::vector<int> to be filled with terrain data. 1 index per tile: [type].
     * @param creatureData A pointer to an empty std::vector<int> to be filled with creature data. 3 indices per tile: [type, direction, controller].
     * @param colorDataVec A pointer to an std::vector<std::vector<float> > to be filled with color data. The outer std::vector should contain an empty std::vector<float> for each client. For each client, there will be 3 indices per tile: [red, green, blue].
     * @param damageData A pointer to an empty std::vector<int> to be filled with damage data. 1 index per tile: [damage].
     * @param offsetData A pointer to an empty std::vector<float> to be filled with offset data. 1 index per tile: [offset].
     * @param buildingData A pointer to an empty std::vector<int> to be filled with building data. 2 indices per tile: [type, controller].
     */
    void getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<std::vector<float> >* colorDataVec, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData);
    
    /*!
     * Changes the active player to the next one in the turn cycle.
     */
    void incrementActivePlayer();
    
};

#endif /* Host_hpp */
