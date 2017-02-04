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
#include "Menu.hpp"

#include "Window.hpp"

//An array of booleans representing if, for each key, if that key is pressed
//Declared here so it can work with static function keyCallback. That function needs to be static
bool keys[1024];

//A boolean representing if the mouse has been clicked, for use in buttons and setting active tiles. This boolean is set in the mouse button callback function. Don't use this to see if the mouse was pressed, use the other boolean
//This is only true right after the mouse is pressed
bool mouseJustPressed = false;

//A boolean representing if the mouse has been clicked, for use in buttons and setting active tiles. This boolean is set in the mouse button callback function. Use this to see if the mouse was pressed
//This is true as long as the mouse is down
bool mouseDown = false;

//A boolean representing if the mouse button has been released, for use with resetting buttons. This boolean is set in the mouse button callback function
bool mouseUp = false;


//Callback functions
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);


//Functions
void updateMouse();

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
            
            //Test menu:
            Window window;
            window.init(800, 600, "Game", false, true);
            window.setKeyCallback(keyCallback);
            window.setMouseButtonCallback(mouseButtonCallback);
            ClientSocket socket;
            Menu M(window, &mouseDown, &mouseUp, keys);
            
            while (!M.getShouldWindowClose()) {
                updateMouse();
                M.render(&socket);
                if (socket.getSet()) {
                    break;
                }
            }
            
            //Run as client
            
//            Window window;
//            window.init(800, 600, "Game", false, true);
//            window.setKeyCallback(keyCallback);
//            window.setMouseButtonCallback(mouseButtonCallback);
            
            Client C(window, &socket, &mouseDown, &mouseUp, keys);
        
            while (!C.getShouldWindowClose()) {
                updateMouse();
                C.render();
            }
            
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
                
                H.board.setBuilding(3, 5, Building(3, 5, 300, 0));
                
                H.board.setBuilding(5, 3, Building(5, 3, 300, 1));
                
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


void updateMouse() {
    if (mouseJustPressed) {
        mouseJustPressed = false;
        mouseDown = true;
    } else {
        mouseDown = false;
    }
}


//A function GLFW can call when a key event occurs
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS && mods == GLFW_MOD_SUPER) { //Command-W: close the application
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

//A function GLFW can call when a key event occurs
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    mouseJustPressed = false;
    mouseUp = false;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseJustPressed = true; //Indicate the mouse has just been pressed. This will then be processed at the start and end of each frame to set mouseDown
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouseUp = true;
    }
}
