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
#include "Visualizer.hpp"

int main(int argc, const char * argv[]) {
//Set up:
    srand((int)time(NULL));
    
//Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLuint x = 0; x < BOARD_WIDTH; x++) {
        std::vector<Tile> row;
        for (GLuint y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 && y == 1)
                row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
            else
                row.push_back(Tile(OPEN_TERRAIN, x, y));
        }
        board.push_back(row);
    }
    
    Visualizer V("Shaders/board/board.vert", "Shaders/board/board.geom", "Shaders/board/board.frag", board);
    
    //Reminder: x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, cost, startDirection, controller

    V.game.board()->setCreature(3, 3, Creature(3, 3, Human, 2, 3, 1, LightMelee, 1, 1, 1, NORTH, 0));

    V.game.board()->setCreature(2, 2, Creature(2, 2, Human, 2, 3, 1, LightMelee, 1, 1, 1, NORTH, 1));
    
    V.game.board()->setBuilding(3, 5, Building(3, 5, 3, 1, 0));
    
    V.game.board()->setBuilding(5, 3, Building(5, 3, 3, 1, 1));
    
//Game loop:
    while(!glfwWindowShouldClose(V.window())) {
        V.render();
    }
    
    V.terminate();

    return 0;
}