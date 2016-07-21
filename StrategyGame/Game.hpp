//
//  Game.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

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
    
    //Public get functions
    
    /*!
     * @return A pointer to the Board object representing the game board.
     */
    Board* board();
    
    /*!
     * @return The current active player.
     */
    unsigned int activePlayer();
    
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
    void updateCreatures(float deltaTime);
    void updateSelected(bool *mouseDown, glm::vec2 cursorPos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]);
    bool moveAdjacent(unsigned int x, unsigned int y, int direction, float deltaTime);
    void incrementActivePlayer();
    glm::ivec2 mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, glm::vec4 tileCenters[NUMBER_OF_TILES]);
    std::vector<Tile> getReachableTiles (Tile creatureTile);
    std::vector<GLuint> getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY);
    
};

#endif /* Game_hpp */
