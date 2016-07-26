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

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

//Local includes
#include "Board.hpp"
#include "Player.hpp"

//Preprocessor Directives
#define BOARD_WIDTH 12
#define NUMBER_OF_TILES BOARD_WIDTH * BOARD_WIDTH
#define INDICES_PER_TILES 2
#define NUMBER_OF_PLAYERS 2

#define NO_SELECTION glm::ivec2(-1, -1)
#define INTERFACE_BOX_SELECTION glm::ivec2(-2, -2)

#define MOVEMENT_CONSOLE_OUTPUT

class Game {
public:
    //Constructor
    Game(Board board);
    
    //Public properties
    
    //Public member functions
    
    /*!
     * Distance formula using Pythagorean Theorem
     */
    static GLfloat getDistance(glm::vec2 point1, glm::vec2 point2);
    
    /*!
     * A function to advance to the next turn.
     */
    void nextTurn();
    
    /*!
     * A function that updates the offset of each creature to cause movement animation. This should be called once every frame.
     *
     * @param deltaTime The time since the last fram, to multiply by the velocity to get a constant distance. This keeps animation speed constant on different machines.
     */
    void updateCreatures(float deltaTime);
    
    /*!
     * A function to update the selected tile based on mouse clicks.
     *
     * @param mouseDown A pointer to the boolean representing if the mouse is down. Note: This function may alter that bool based on if the mouse click has been dealt with.
     * @param cursorPos A glm::vec2 of the cursor position, in screen coordinates. Can be obtained from glfwGetCursorPos.
     * @param windowSize A glm::ivec2 representing the window size, in screen coordinates. Can be obtained from glfwGetWindowSize.
     * @param tileCenters An array of glm::vec4 of length NUMBER_OF_TILES representing the center of each tile, after transformations. This can be gotten by manipulating the vertex data and multiplying it with the transformation matrices.
     */
    void updateSelected(bool *mouseDown, glm::vec2 cursorPos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]);
    
    //Public get functions
    
    /*!
     * @return A pointer to the Board object representing the game board.
     */
    Board* board();
    
    /*!
     * @return The current active player.
     */
    unsigned int activePlayer();
    
    /*!
     * @return The current selected tile, in the form of a glm::ivec2. The coordinates are in terms of board coordinates.
     */
    glm::ivec2 tileSelected();
    
private:
    
    //Private properties
    
    //Board data
    Board gameBoard;
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    //Player and turn data
    Player players[NUMBER_OF_PLAYERS];
    unsigned int currentActivePlayer = 0;
    unsigned int turn = 1;
    
    //Private member functions
    
    bool moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime);
    
    void incrementActivePlayer();
    
    /*!
     * A function to calculate the tile closest to the mouse location at any given point in time.
     * (-1, -1) is returned if the selection was outside of the board.
     * Possible errors include if the board size is below 2x2, because calculations require a board size at least that large.
     *
     * @param mousePos A glm::vec2 of the cursor position, in screen coordinates. Can be obtained from glfwGetCursorPos.
     * @param windowSize A glm::ivec2 representing the window size, in screen coordinates. Can be obtained from glfwGetWindowSize.
     * @param tileCenters An array of glm::vec4 of length NUMBER_OF_TILES representing the center of each tile, after transformations. This can be gotten by manipulating the vertex data and multiplying it with the transformation matrices.
     *
     * @return The tile indices in the board, the 2D vector. In the form of a glm vector of 2 ints (glm::ivec2).
     */
    glm::ivec2 mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]);
    
    std::vector<Tile> getReachableTiles (Tile creatureTile);
    
    std::vector<GLuint> getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY);
    
};

#endif /* Game_hpp */
