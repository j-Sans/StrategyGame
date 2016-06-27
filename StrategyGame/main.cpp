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

//OpenGL additional classes includes
//#include "GLextensions/Shader.hpp"

//Local includes
#include "Tile.hpp"
#include "Game.hpp"


//Variables:

//Board speed
const GLuint boardWidth = 10;


int main(int argc, const char * argv[]) {
    //Set up
    srand((int)time(NULL));
    
//Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLuint a = 0; a < boardWidth; a++) {
        std::vector<Tile> row;
        for (GLuint b = 0; b < boardWidth; b++) {
            if (a == 2 && b == 2)
                row.push_back(Tile(MOUNTAIN_TERRAIN, a, b));
            else
                row.push_back(Tile(OPEN_TERRAIN, a, b));
        }
        board.push_back(row);
    }
    
    Game G("Shaders/board.vert", "Shaders/board.geom", "Shaders/board.frag", board);
    
    //Game loop
    while(!glfwWindowShouldClose(G.window())) {
        G.render();
    }
    
    G.terminate();

    return 0;
}