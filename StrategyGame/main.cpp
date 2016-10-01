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
#include "Client.hpp"
#include "Host.hpp"

int main(int argc, const char * argv[]) {
//Set up:
    srand((int)time(NULL));
    
    bool host;
    
    bool repeat = true;
    while (repeat) {
        repeat = false;
        std::cout << "Run as host? Type \"Y\" for yes or \"N\" for no" << std::endl;
        std::string input;
        std::cin >> input;
        if (input == "Y" || input == "y")
            host = true;
        else if (input == "N" || input == "n")
            host = false;
        else
            repeat = true;
    }

    if (!host) {
        Client C("localhost", 3000);
        
        while (!glfwWindowShouldClose(V.window()))
            V.render();
        
        V.terminate();
    } else {
        //Gameboard:
        std::vector<std::vector<Tile> > board;
        for (GLuint x = 0; x < BOARD_WIDTH; x++) {
            std::vector<Tile> row;
            for (GLuint y = 0; y < BOARD_WIDTH; y++) {
                if (x == 0 && y == 1)
                    row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
                else if ((x == 3  && y < 4) || (x == 4  && y < 5) || (x == 5 && y < 3) || (y > BOARD_WIDTH - 4) || (x == BOARD_WIDTH - 1) || (x == BOARD_WIDTH - 2 && y == BOARD_WIDTH - 4) || (x == BOARD_WIDTH - 2 && y == BOARD_WIDTH - 5) || (x == BOARD_WIDTH - 3 && y == BOARD_WIDTH - 4) || (x == BOARD_WIDTH - 2 && y == BOARD_WIDTH - 6) || (x == 6 && y == 0)) {
                    row.push_back(Tile(FOREST_TERRAIN, x, y));
                } else
                    row.push_back(Tile(OPEN_TERRAIN, x, y));
            }
            board.push_back(row);
        }
        
        Host H(1, 3000, Board(board));
        
        while (true)
            H.update();
    }
    
//    //Reminder: x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, cost, startDirection, controller
//
//    V.game.board()->setCreature(5, 4, Creature(3, 3, Human, 300, 3, 40, LightMelee, 1, 2, 1, NORTH, 0));
//
//    V.game.board()->setCreature(2, 2, Creature(2, 2, Human, 300, 3, 40, LightMelee, 1, 2, 1, NORTH, 1));
//    
//    V.game.board()->setBuilding(3, 5, Building(3, 5, "Make creature", "building_new_creature(3,5)", 300, 1, 0));
//    
//    V.game.board()->setBuilding(5, 3, Building(5, 3, "Make creature", "building_new_creature(5,3)", 300, 1, 1));
//    
////Game loop:
//    while(!glfwWindowShouldClose(V.window())) {
//        V.render();
//    }
//    
//    V.terminate();

    return 0;
}
