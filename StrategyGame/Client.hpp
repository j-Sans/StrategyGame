//
//  Client.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

//Standard library includes
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <list>
#include <map>
#include <thread>

//Local includes
#include "Board.hpp"
#include "Visualizer.hpp"

//Macros
#define NO_SELECTION glm::ivec2(-1, -1)
#define INTERFACE_BOX_SELECTION glm::ivec2(-2, -2)

#define TILE_STYLE 0
#define TILE_HOVER 1

#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define GREY glm::vec3(0.625f, 0.625f, 0.625f)
#define RED glm::vec3(1.0f, 0.625f, 0.625f)
#define YELLOW glm::vec3(1.0f, 1.0f, 0.5f)
#define GREEN glm::vec3(0.62f, 1.0f, 0.625f)
#define CYAN glm::vec3(0.625f, 1.0f, 1.0f)
#define BLUE glm::vec3(0.625f, 0.625f, 1.0f)
#define PURPLE glm::vec3(0.5f, 0.1f, 0.9f)

#define HOVER_EFFECT glm::vec3(0.75f, 0.75f, 0.75f)

//TILE_STYLE
#define REGULAR 0
#define SELECTED 1
#define ATTACKABLE 2
#define REACHABLE 3

//HOVER
#define NO_HOVERING 0
#define HOVERING 1

class Client {
public:
    
    //Constructor
    Client(Window* w, ClientSocket* socket, bool* mouseDown, bool* mouseUp, bool* returnToMenu, bool* keys);
    
    //Public member functions
    
    void render();
    
    bool getShouldWindowClose();
    
private:
    //Private properties
    
    bool* returnToMenu;
    
    //Board info
    Board board;
    
    //Info unique to client
    unsigned int playerNum;
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    std::vector<std::vector<std::array<int, 2> > > boardInfo; //Contains an array of ints (represented by macros) that give information:
    /*
     * [0]: Tile style (whether it is selected, attackable, reachable, etc...)
     * [1]: Whether the mouse is hovering, either as TRUE or FALSE
     */
    
    std::vector<std::vector<std::queue<std::string> > > tileActions; //For each tile, contains a queue of action strings, the first to be done when the tile is clicked if it is reachable
    
    ClientSocket* socket;
    
    //Visualizer, to render the window
    Visualizer visualizer;
    
    //Interfaces
    std::map<interfaceType, Interface> interfaces;
    std::string announcementStr = "Defeat the enemy tower!";
    
    //Information for host
    std::list<std::string> actionsForClientInfo;
    
    
    //Private member functions
    
    void resetAllTiles();
    
    void updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int currentTime);
    
    void setInterfaces();
    
    /*!
     * Set the correct interfaces to render based on the selected tile. Should be called every frame.
     */
    void updateInterfaces();
    
    /*!
     * Fill given std::vectors with OpenGL data.
     *
     * @param terrainData A pointer to an empty std::vector<int> to be filled with terrain data. 1 index per tile: [type].
     * @param creatureData A pointer to an empty std::vector<int> to be filled with creature data. 3 indices per tile: [type, direction, controller].
     * @param colorData A pointer to an empty std::vector<float> to be filled with coloro data. 3 indices per tile: [red, green, blue].
     * @param damageData A pointer to an empty std::vector<int> to be filled with damage data. 1 index per tile: [damage].
     * @param offsetData A pointer to an empty std::vector<float> to be filled with offset data. 1 index per tile: [offset].
     * @param buildingData A pointer to an empty std::vector<int> to be filled with building data. 2 indices per tile: [type, controller].
     */
    void getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<float>* colorData, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData);
    
    /*!
     * Get the color of a specific tile from the board. An error may be thrown if the passed in location is out of bounds.
     *
     * @param x The x location of the tile.
     * @param y The y location of the tile.
     *
     * @return The color of the tile at the specified location, with colors being in RGB from 0 to 1 as floats.
     */
    glm::vec3 tileColor(unsigned int x, unsigned int y);
    
    /*!
     * A function to process a local action. This may include button actions. Any action to be performed on the host will be added to actionsForClientInfo.
     *
     * @param An std::string representing the action to be done.
     */
    void processAction(std::string action);
    
    /*!
     * A function to process an action received from the host. This likely will be an announcement.
     *
     * @param An std::string representing the action to be done.
     */
    void processFromHost(std::string action);
    
    void resolveTileAction(int x, int y);
};

#endif /* Client_hpp */
