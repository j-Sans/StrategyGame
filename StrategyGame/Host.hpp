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
#include <mutex>
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
     * @param gameBoard A board to be used as the game board. Passed by value, so the game board will actually be stored within the host class.
     */
    Host(Board gameBoard);
    
    //Public properties
    Board board;
    
    //Clients
    ServerSocket socket; //Public so that it can be accessed by main() for communicating with the first client
    
    //Public member functions
    
    static std::string storeVectorOfInts(std::vector<int> vec);
    
    static std::string storeVectorOfFloats(std::vector<float> vec);
    
    /*!
     * A function that must be called before any other methods can be used. Initializes the socket of the game. If the optional parameter is nonzero, the game will automatically begin.
     *
     * @param portNum The port on this system (the server side) where to start taking connections. Succeeding ports may also be used, depending on implementation and number of players.
     * @param numberOfPlayers An optional parameter indicating the number of players with which to initialize the game. If unset it is set as 0. Otherwise (if numberOfPlayers != 0) then Host will automatically begin.
     */
    void set(int portNum, unsigned int numberOfPlayers = 0);
    
    /*!
     * Adds a player to the game and initializes the connection socket with that player.
     */
    void addPlayer();
    
    /*!
     * Begin the clock for the game and send out initial information to each client. Automatically called by constructor if numberOfPlayers != 0.
     */
    void begin();
    
    /*!
     * Updates the players and the game based on input from clients. This should be called every frame.
     */
    void update();
    
    /*!
     * @return The number of players currently connected.
     */
    unsigned int getNumberPlayers();
    
    /*!
     * Serialize this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    std::string serialize();
    
private:
    //Private properties
    
    //Time
    std::chrono::time_point<std::chrono::steady_clock> programStartTime;
    std::chrono::duration<float> lastFrame;
    std::chrono::duration<float> lastUpkeep;
    
    constexpr static const float timeBetweenUpkeep = 5.0f;
    
//    float timeOfLastUpkeep = 0;
    
    float deltaTime = 0.0f;
    
    //Game
    std::vector<Player> players;
    std::vector<std::pair<bool, bool> > alivePlayers; // { alive, connected }
    
    bool setUp = false;
    
    //Private member functions
    
    /*!
     * Follow an action based on a std::string sent from the client. The string comes from the clientInfo string.
     *
     * @param action The string telling what action to do.
     * @param playerNum The number of the player who sent the action. -1 can be used to indicate that it is irrelevant.
     */
    void processAction(std::string action, int playerNum);
    
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
     * Remove a player from the game. An error may be thrown if the playerNum is not a valid player.
     *
     * @param The player number of the player to remove.
     */
    void losePlayer(int playerNum);
    
    /*!
     * Broadcasts a given message to all remaining alive players.
     *
     * @param message The std::string to send.
     */
    void broadcast(std::string message);
    
    /*!
     * A function to see if all remaining alive players sent the given string.
     *
     * @param str The std::string to compare to.
     *
     * @return True if each the string was received from all active players. False otherwise.
     */
    bool receivedFromAll(std::string str);
    
    /*!
     * Changes the active player to the next one in the turn cycle.
     */
//    void incrementActivePlayer();
    
};

#endif /* Host_hpp */
