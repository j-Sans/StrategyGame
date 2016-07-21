//
//  Game.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

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
    
    //Public get functions
    
    /*!
     * @return The Board object representing the game board.
     */
    Board board();
    
private:
    
    //Private properties
    
    //Board data
    Board gameBoard;
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    //Player and turn data
    Player players[NUMBER_OF_PLAYERS];
    unsigned int activePlayer = 0;
    unsigned int turn = 1;
    
};

#endif /* Game_hpp */
