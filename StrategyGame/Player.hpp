//
//  Player.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Macros.h"

//Standard library includes
#include <array>
#include <queue>
#include <string>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

//Local includes
#include "Board.hpp"

//Preprocessor Directives
#define BOARD_WIDTH 12
#define NUMBER_OF_TILES BOARD_WIDTH * BOARD_WIDTH
#define INDICES_PER_TILES 2
#define NUMBER_OF_PLAYERS 2

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

class Player {
public:
    //Constructor
    Player(Board* board, unsigned int num);
    
    //Public properties
    
    Board* board;
    
    //Public member functions
    
    /*!
     * Reset the style of all tiles.
     */
    void resetAllTiles();
    
    /*!
     * A function to set the selected tile. If the x or y is out of range and the input isn't NO_SELECTION or INTERFACE_BOX_SELECTION, then nothing will happen and false will be returned. To pass NO_SELECTION or other glm::ivec2 macros as arguments, do this: selectTile(NO_SELECTION.x, NO_SELECTION.y);
     *
     * @param x An unsigned int representing the x location of the tile on the board.
     * @param y An unsigned int representing the y location of the tile on the board.
     *
     * @return Whether the tile was successfully selected.
     */
    bool selectTile(unsigned int x, unsigned int y);
    
    /*!
     * A function to set the style tile. If the x or y is out of range, then nothing will happen and false will be returned.
     *
     * @param x An unsigned int representing the x location of the tile on the board.
     * @param y An unsigned int representing the y location of the tile on the board.
     * @param style An unsigned int representing the style to set for the given tile.
     *
     * @return Whether the tile was successfully given the new style.
     */
    bool setStyle(unsigned int x, unsigned int y, unsigned int style);
    
    /*!
     * A function that updates the offset of each creature to cause movement animation. This should be called once every frame.
     *
     * @param deltaTime The time since the last fram, to multiply by the velocity to get a constant distance. This keeps animation speed constant on different machines.
     */
    void updateCreatures(float deltaTime);
    
    /*!
     * A function to update the selected tile based on mouse clicks.
     *
     * @param mouseDown A boolean representing if the mouse is down. 
     * @param mousePos A glm::ivec2 representing the tile the mouse is currently at. This should be calculated on the client side and sent to the server.
     * @param currentTime An unsigned int representing the time of the current grame.
     */
    void updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int currentTime);
    
    /*!
     * A function to calculate the tile closest to the mouse location at any given point in time.
     * (-1, -1) is returned if the selection was outside of the board.
     * Possible errors include if the board size is below 2x2, because calculations require a board size at least that large.
     *
     * @param mousePos A glm::vec2 of the cursor position, in screen coordinates. Can be obtained from glfwGetCursorPos.
     * @param windowSize A glm::ivec2 representing the window size, in screen coordinates. Can be obtained from glfwGetWindowSize.
     * @param tileCenters An std::vector of glm::vec4 (should be length NUMBER_OF_TILES) representing the center of each tile, after transformations. This can be gotten by manipulating the vertex data and multiplying it with the transformation matrices.
     *
     * @return The tile indices in the board, the 2D vector. In the form of a glm vector of 2 ints (glm::ivec2).
     */
    glm::ivec2 mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, std::vector<glm::vec4> tileCenters);
    
    //Public get functions
    
    /*!
     * @return The current selected tile, in the form of a glm::ivec2. The coordinates are in terms of board coordinates.
     */
    glm::ivec2 tileSelected();
    
    /*!
     * @return The color of the tile at the specified location.
     */
    glm::vec3 tileColor(unsigned int x, unsigned int y);
    
    /*!
     * A function to check if a creature can be moved to the destination from its current location. An invalid_argument exception will be thrown if there is no creature, and a range_error if either set of coordinates is not on the board.
     *
     * @param destination A glm::ivec2 representing the board coordinates of the destination tile.
     * @param currentLoc A glm::ivec2 representing the board coordinates of the creature's current location.
     *
     * @return Whether the creature is able to move there.
     */
    bool destinationInRange(glm::ivec2 destination, glm::ivec2 currentLoc);
    
    /*!
     * A function to check if a creature can attack the destination from its current location. An invalid_argument exception will be thrown if there is no creature at either destination or the current location, a range_error if either set of coordinates is not on the board, and a logic_error if the creature has no energy.
     *
     * @param destination A glm::ivec2 representing the board coordinates of the defender's tile.
     * @param currentLoc A glm::ivec2 representing the board coordinates of the creature's current location.
     *
     * @return Whether the creature is able to attack.
     */
    bool attackInRange(glm::ivec2 destination, glm::ivec2 currentLoc);
    
    std::vector<GLuint> getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY);
    
private:
    
    //Private properties
    
    //Player Ddya
    unsigned int playerNum;
    
    //Board data
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    std::vector<std::vector<std::array<int, 2> > > boardInfo; //Contains an array of ints (represented by macros) that give information:
    /*
     * [0]: Tile style (whether it is selected, attackable, reachable, etc...)
     * [1]: Whether the mouse is hovering, either as TRUE or FALSE
     */
    
    std::vector<std::vector<std::queue<std::string> > > tileActions; //For each tile, contains a queue of action strings, the first to be done when the tile is clicked if it is reachable
    
    //Private member functions
    
    /*!
     * Resolve the next tile action at the given location
     *
     * @param x The x coordinate of the tile to resolve.
     * @param y The y coordinate of the tile to resolve.
     */
    void resolveTileAction(unsigned int x, unsigned int y);
    
    bool moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime);
    
    /*!
     * A function to select a creature and and make nearby tiles properly stylized (reachable ones and attackable, for instance). If an error occurs (see below) no error is thrown, instead false is returned and nothing happens.
     *
     * @param x An unsigned int representing the x location of the creature on the board.
     * @param y An unsigned int representing the y location of the creature on the board.
     *
     * @return Whether the creature was successfully selected or if an error prevented this. Errors include if x or y is out of range, or if there is no creature at the designated spot.
     */
    bool selectCreature(unsigned int x, unsigned int y);
    
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
};

#endif /* Player_hpp */
