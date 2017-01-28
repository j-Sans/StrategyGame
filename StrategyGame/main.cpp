//
//  main.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

//C library includes
#include <stdlib.h>
#include <ctime>

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
    srand((int)std::time(NULL));
    
    bool repeat = true;
    while (repeat) {
        repeat = false;
        std::cout << "Run as host? For yes, type a number between 1 and 5 to be the number of clients. Otherwise, type \"N\" or \"n\" for no" << std::endl;
        std::string input;
        std::cin >> input;
        if (input == "N" || input == "n") {
            
            //Run as client
            
            Client C("localhost", 3000);
        
            while (!C.getShouldWindowClose())
                C.render();
            
            C.terminate();
            //send message to other clients that the player has left the game.
        } else {
            int numPlayers;
            try {
                numPlayers = std::stoi(input);
            } catch (std::invalid_argument e) {
                repeat = true;
                std::cout << "There was an error processing your answer. Please try another input" << std::endl;
                continue;
            }
            
            if (numPlayers >= 1 && numPlayers <= 5) {
                
                //Run as host
                
                //Gameboard:
                std::vector<std::vector<Tile> > board;
                for (GLuint x = 0; x < BOARD_WIDTH; x++) {
                    std::vector<Tile> row;
                    for (GLuint y = 0; y < BOARD_WIDTH * 2; y++) {
                        if (x + (2 * y) < 5)
                            row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
                        else if (x + y > 15)
                            row.push_back(Tile(FOREST_TERRAIN, x, y));
                        else
                            row.push_back(Tile(OPEN_TERRAIN, x, y));
                    }
                    board.push_back(row);
                }
                
                Host H(numPlayers, 3000, Board(board));
                
                //Reminder: Creature(x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, startDirection, controller)
                
                for(int a = 0; a < numPlayers; a++) {
                    H.board.setCreature(a + 2, a + 2, Creature(a + 2, a + 2, Human, 300, 3, 40, Melee, 1, 1, NORTH, a));
                }
                
                H.board.setBuilding(3, 5, Building(3, 5, "Make creature", "building_new_creature(3,5)", 300, 0));
                
                H.board.setBuilding(5, 3, Building(5, 3, "Make creature", "building_new_creature(5,3)", 300, 1));
                
                while (true)
                    H.update();
                
                return 0;
                
            } else {
                repeat = true;
                std::cout << "There was an error processing your answer. Please try another input" << std::endl;
            }
        }
    }

    return 0;
}
