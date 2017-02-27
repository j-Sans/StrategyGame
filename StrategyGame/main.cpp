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
void host(int numPlayers);


int main(int argc, const char * argv[]) {
//Set up:
    srand((int)std::time(NULL));
    
    std::thread hostThread;
    Host* hostPointer = nullptr;
    
    bool repeat = true;
    while (repeat) {
        repeat = false;
        std::cout << "Run as host? For yes, type a number between 1 and 2 to be the number of clients. Otherwise, type \"N\" or \"n\" for no" << std::endl;
        std::string input;
        std::cin >> input; 
        if (input == "N" || input == "n") {
            
            //Test menu:
            Window window;
            window.init(800, 600, "Game", false, true);
            window.setKeyCallback(keyCallback);
            window.setMouseButtonCallback(mouseButtonCallback);
            
            while (true) {
                ClientSocket socket;
                Menu M(&window, &socket, &mouseDown, &mouseUp, keys);
                
                bool runningHost = false;
                while (!M.getShouldWindowClose()) {
                    updateMouse();
                    M.render();
                    int action = M.getStatus();
                    if (action == READY_TO_PLAY) {
                        if (runningHost) {
                            hostPointer->begin();
                        }
                        break;
                    } else if (action == PLAY_AS_HOST) {
                        if (!runningHost) {
                            hostThread = std::thread(host, 1, hostPointer);
                            runningHost = true;
                        }
                        if (hostPointer != nullptr) {
                            M.numberOfConnections = hostPointer->getNumberPlayers();
                        }
                    }
                }
                
                if (window.shouldClose()) {
                    window.terminate();
                    return 0;
//                    break;
                }
                
                bool returnToMenu = false;
                
                Client C(&window, &socket, &mouseDown, &mouseUp, &returnToMenu, keys);
                
                while (!C.getShouldWindowClose() && !returnToMenu) {
                    updateMouse();
                    C.render();
                }
                
                if (window.shouldClose()) {
                    window.terminate();
                    return 0;
//                    break;
                }
                //send message to other clients that the player has left the game.
            }
        } else {
            int numPlayers;
            try {
                numPlayers = std::stoi(input);
            } catch (std::invalid_argument e) {
                repeat = true;
                std::cout << "There was an error processing your answer. Please try another input" << std::endl;
                continue;
            }
            
            if (numPlayers >= 1 && numPlayers <= 2) {
                
                //Run as host
                
                //Gameboard:
                std::vector<std::vector<Tile> > board;
                for (GLint x = 0; x < BOARD_WIDTH; x++) {
                    std::vector<Tile> row;
                    for (GLint y = 0; y < BOARD_WIDTH; y++) {
                        if ((x == 2 && y == BOARD_WIDTH - 1 - 2) || (x == 3 && y == BOARD_WIDTH - 1 - 3) || (x == 4 && y == BOARD_WIDTH - 1 - 4) || (x == BOARD_WIDTH - 1 - 2 && y == 2) || (x == BOARD_WIDTH - 1 - 3 && y == 3) || (x == BOARD_WIDTH - 1 - 4 && y == 4))
                            row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
                        else if (x >= y - 1 && x <= y + 1)
                            row.push_back(Tile(FOREST_TERRAIN, x, y));
                        else
                            row.push_back(Tile(OPEN_TERRAIN, x, y));
                    }
                    board.push_back(row);
                }
                
                Host H(numPlayers, 3000, Board(board));
                
                //Reminder: Creature(x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, startDirection, controller)
                
                H.board.setCreature(Creature(0, BOARD_WIDTH - 2, Human, 3, 3, 2, Melee, 1, 30, NORTH, 0));
                H.board.setCreature(Creature(1, BOARD_WIDTH - 2, Human, 3, 3, 2, Melee, 1, 1, NORTH, 0));
                H.board.setCreature(Creature(1, BOARD_WIDTH - 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 0));
                
                H.board.setCreature(Creature(BOARD_WIDTH - 1, 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
                H.board.setCreature(Creature(BOARD_WIDTH - 2, 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
                H.board.setCreature(Creature(BOARD_WIDTH - 2, 0, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
                
                Building player0Home(0, BOARD_WIDTH - 1, 10, 0);
                player0Home.deathAction = "player_lose_0";
                Building player1Home(BOARD_WIDTH - 1, 0, 1, 1);
                player1Home.deathAction = "player_lose_1";
                
                H.board.setBuilding(player0Home);
                H.board.setBuilding(player1Home);
                
                while (true)
                    H.update();
                
                return 0;
                
            } else {
                repeat = true;
                std::cout << "There was an error processing your answer. Please try another input" << std::endl;
            }
        }
    }
    
    hostThread.join();
    
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

void host(int numPlayers, Host* hostPointer) {
    //Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLint x = 0; x < BOARD_WIDTH; x++) {
        std::vector<Tile> row;
        for (GLint y = 0; y < BOARD_WIDTH; y++) {
            if ((x == 2 && y == BOARD_WIDTH - 1 - 2) || (x == 3 && y == BOARD_WIDTH - 1 - 3) || (x == 4 && y == BOARD_WIDTH - 1 - 4) || (x == BOARD_WIDTH - 1 - 2 && y == 2) || (x == BOARD_WIDTH - 1 - 3 && y == 3) || (x == BOARD_WIDTH - 1 - 4 && y == 4))
                row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
            else if (x >= y - 1 && x <= y + 1)
                row.push_back(Tile(FOREST_TERRAIN, x, y));
            else
                row.push_back(Tile(OPEN_TERRAIN, x, y));
        }
        board.push_back(row);
    }
    
    Host H(numPlayers, 3000, Board(board));
    
    hostPointer = &H;
    
    //Reminder: Creature(x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, startDirection, controller)
    
    H.board.setCreature(Creature(0, BOARD_WIDTH - 2, Human, 3, 3, 2, Melee, 1, 30, NORTH, 0));
    H.board.setCreature(Creature(1, BOARD_WIDTH - 2, Human, 3, 3, 2, Melee, 1, 1, NORTH, 0));
    H.board.setCreature(Creature(1, BOARD_WIDTH - 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 0));
    
    H.board.setCreature(Creature(BOARD_WIDTH - 1, 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
    H.board.setCreature(Creature(BOARD_WIDTH - 2, 1, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
    H.board.setCreature(Creature(BOARD_WIDTH - 2, 0, Human, 3, 3, 2, Melee, 1, 1, NORTH, 1));
    
    Building player0Home(0, BOARD_WIDTH - 1, 10, 0);
    player0Home.deathAction = "player_lose_0";
    Building player1Home(BOARD_WIDTH - 1, 0, 1, 1);
    player1Home.deathAction = "player_lose_1";
    
    H.board.setBuilding(player0Home);
    H.board.setBuilding(player1Home);
    
    while (true)
        H.update();
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
