//
//  Game.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "Macros.h"

//Standard library includes
#include <array>

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

#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define GREY glm::vec3(0.625f, 0.625f, 0.625f)
#define RED glm::vec3(1.0f, 0.625f, 0.625f)
#define YELLOW glm::vec3(1.0f, 1.0f, 0.5f)
#define GREEN glm::vec3(0.62f, 1.0f, 0.625f)
#define CYAN glm::vec3(0.625f, 1.0f, 1.0f)
#define BLUE glm::vec3(0.625f, 0.625f, 1.0f)
#define PURPLE glm::vec3(0.5f, 0.1f, 0.9f)

#define REGULAR 0
#define SELECTED 1
#define ATTACKABLE 2
#define REACHABLE 3

class Game {
public:
    //Constructor
    Game(Board* board);
    
    //Public properties
    
    //Public member functions
    
    /*!
     * A function to set the selected tile. If the x or y is out of range and the input isn't NO_SELECTION or INTERFACE_BOX_SELECTION, then nothing will happen and false will be returned. To pass NO_SELECTION or other glm::ivec2 macros as arguments, do this: selectTile(NO_SELECTION.x, NO_SELECTION.y);
     *
     * @param x An int representing the x location of the tile on the board.
     * @param y An int representing the y location of the tile on the board.
     *
     * @return Whether the tile was successfully selected.
     * @param activePlayer An unsigned int representing the index of the active player.
     */
    bool selectTile(int x, int y, unsigned int activePlayer);
    
    /*!
     * A function that updates the offset of each creature to cause movement animation. This should be called once every frame.
     *
     * @param deltaTime The time since the last fram, to multiply by the velocity to get a constant distance. This keeps animation speed constant on different machines.
     * @param activePlayer An unsigned int representing the index of the active player.
     */
    void updateCreatures(float deltaTime, unsigned int activePlayer);
    
    /*!
     * A function to update the selected tile based on mouse clicks.
     *
     * @param mouseDown A boolean representing if the mouse is down. 
     * @param mousePos A glm::ivec2 representing the tile the mouse is currently at. This should be calculated on the client side and sent to the server.
     * @param activePlayer An unsigned int representing the index of the active player.
     */
    void updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int activePlayer);
    
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
     * @return A pointer to the Board object representing the game board.
     */
    Board* board();
    
    /*!
     * @return The current selected tile, in the form of a glm::ivec2. The coordinates are in terms of board coordinates.
     */
    glm::ivec2 tileSelected();
    
    /*!
     * @return The color of the tile at the specified location.
     */
    glm::vec3 tileColor(unsigned int x, unsigned int y);
    
private:
    
    //Private properties
    
    //Board data
    Board *gameBoard;
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    std::vector<std::vector<std::array<int, 1> > > boardInfo; //Contains an array of ints (represented by macros) that give information:
    /*
     * [0]: Tile style (whether it is selected, attackable, reachable, etc...)
     */
    
    
    //Private member functions
    
    bool moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime);
    
    /*!
     * A function to select a creature and and make nearby tiles properly stylized (reachable ones and attackable, for instance). If an error occurs (see below) no error is thrown, instead false is returned and nothing happens.
     *
     * @param x An unsigned int representing the x location of the creature on the board.
     * @param y An unsigned int representing the y location of the creature on the board.
     * @param activePlayer An unsigned int representing the index of the active player.
     *
     * @return Whether the creature was successfully selected or if an error prevented this. Errors include if x or y is out of range, or if there is no creature at the designated spot.
     */
    bool selectCreature(unsigned int x, unsigned int y, unsigned int activePlayer);
    
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
    
    std::vector<GLuint> getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY);
    
    //Private get functions
    
};

#endif /* Game_hpp */
