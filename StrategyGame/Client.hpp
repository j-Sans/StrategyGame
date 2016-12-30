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
    Client(std::string hostName, int portNum);
    
    //Public member functions
    
    void render();
    void terminate();
    
    bool windowShouldClose();
    
private:
    //Private properties
    
    //Board info
    Board board;
    
    //Info unique to clinet
    unsigned int playerNum;
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    std::vector<std::vector<std::array<int, 2> > > boardInfo; //Contains an array of ints (represented by macros) that give information:
    /*
     * [0]: Tile style (whether it is selected, attackable, reachable, etc...)
     * [1]: Whether the mouse is hovering, either as TRUE or FALSE
     */
    
    std::vector<std::vector<std::queue<std::string> > > tileActions; //For each tile, contains a queue of action strings, the first to be done when the tile is clicked if it is reachable
    
    ClientSocket socket;
    
    //Visualizer, to render the window
    Visualizer visualizer;
    
    //Information for host
    std::list<std::string> actionsForClientInfo;
    
    //Private member functions
    
    void resetAllTiles();
    
    void updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int currentTime);
    
    /*!
     * A function that gets all of the tiles that a creature can reach.
     *
     * @param creature The tile containing the specified creature that is going to be moving.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be reached.
     */
    std::vector<Tile> getReachableTiles(Tile creatureTile);
    
    /*!
     * A function that gets all of the tiles that a creature can attack.
     *
     * @param creature The tile containing the specified creature that is going to be moving.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be attacked.
     */
    std::vector<Tile> getAttackableTiles(Tile creatureTile);
    
    void resolveTileAction(int x, int y);
};

#endif /* Client_hpp */
