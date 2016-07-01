//
//  main.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

//C library includes
#include <stdlib.h>
#include <time.h>

//Standard library includes
#include <iostream>
#include <vector>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//SOIL: Sets up texture loading
#include <SOIL/SOIL.h>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

//Local includes
#include "Tile.hpp"
#include "Game.hpp"


int main(int argc, const char * argv[]) {
    //Set up
    srand((int)time(NULL));
    
//Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLuint x = 0; x < BOARD_WIDTH; x++) {
        std::vector<Tile> row;
        for (GLuint y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 && y == 2)
                row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
            else
                row.push_back(Tile(OPEN_TERRAIN, x, y));
        }
        board.push_back(row);
    }
    
    Game G("Shaders/board.vert", "Shaders/board.geom", "Shaders/board.frag", board);
    
    G.gameBoard.setCreature(1, 1, Creature(Human, 1, 1, 1, 1, 1, 1, North));
    
    //Game loop
    while(!glfwWindowShouldClose(G.window())) {
        G.render();
    }
    
    G.terminate();

    return 0;
}