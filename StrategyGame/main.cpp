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
void resizeCallback(GLFWwindow *window, int width, int height);


//Functions
void updateMouse();
void host(bool* done);
void threadAddPlayer(bool *done, bool *failed, Host* host);


int main(int argc, const char * argv[]) {
//Set up:
    srand((int)std::time(NULL));
    
    std::thread hostThread;
    bool runningHost = false; //Indicates if this is the client running a host in parallel
    bool finishedRunning = false; //Only if running as host, to indicate when to stop running
    
    //Test menu:
    Window window;
    window.init(800, 600, "Game", true, true);
    window.setKeyCallback(keyCallback);
    window.setMouseButtonCallback(mouseButtonCallback);
    window.setResizeCallback(resizeCallback);
    
    while (true) {
        runningHost = false;
        finishedRunning = false;
        
        ClientSocket socket;
        Menu M(&window, &socket, &mouseDown, &mouseUp, keys);
        
        while (!M.getShouldWindowClose()) {
            updateMouse();
            M.render();
            int action = M.getStatus();
            if (action == READY_TO_PLAY) {
                if (runningHost) {
                    if (!socket.getSet()) {
                        socket.setSocket("localhost", 3000);
                    }
                    socket.send("run:begin()");
                    //No confirmation wanted because the client will receive initial data soon anyway, so that can be like a confirmation
                }
                break;
            } else if (action == PLAY_AS_HOST) {
                if (runningHost == false) {
                    hostThread = std::thread(host, &finishedRunning);
                    
                    if (!socket.getSet()) {
                        socket.setSocket("localhost", 3000);
                    }
                    
                    runningHost = true;
                }
                if (!socket.getSet()) {
                    socket.setSocket("localhost", 3000);
                }
                socket.send("send_number_of_players");
                M.numberOfConnections = std::stoi(socket.receive());
                
            } else if (action == ADD_PLAYER) {
                if (!socket.getSet()) {
                    socket.setSocket("localhost", 3000);
                }
                socket.send("run:addPlayer()");
                if (socket.receive() != "message_received") {
                    throw std::runtime_error("Error communicating with host");
                }
            }
        }
        
        if (window.shouldClose()) {
            window.terminate();
            if (runningHost) {
                finishedRunning = true;
                if (hostThread.joinable()) hostThread.join();
            }
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
            if (runningHost) {
                finishedRunning = true;
                hostThread.join();
            }
            return 0;
//                    break;
        }
        if (runningHost) {
            finishedRunning = true;
            hostThread.join();
            
            if (returnToMenu) {
                runningHost = false;
            }
        }
        
        //send message to other clients that the player has left the game.
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

void host(bool* done) {
    //Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLint x = 0; x < BOARD_WIDTH; x++) {
        std::vector<Tile> row;
        for (GLint y = 0; y < BOARD_WIDTH; y++) {
            if ((x == 3 && y == BOARD_WIDTH - 1 - 3) || (x == 4 && y == BOARD_WIDTH - 1 - 4) || (x == BOARD_WIDTH - 1 - 3 && y == 3) || (x == BOARD_WIDTH - 1 - 4 && y == 4))
                row.push_back(Tile(MOUNTAIN_TERRAIN, x, y));
            else if (x >= y - 1 && x <= y + 1)
                row.push_back(Tile(FOREST_TERRAIN, x, y));
            else
                row.push_back(Tile(OPEN_TERRAIN, x, y));
        }
        board.push_back(row);
    }
    
    Host H(board);
    H.set(3000);
    
    //Reminder: Creature(x, y, Race, maxHealth, maxEnergy, attack, attackStyle, vision, range, startDirection, controller)
    
    int boardBack = BOARD_WIDTH - 1; //Represents the opposite index of the board.
    
    //Player 1
    
    H.board.setCreature(Creature(0, boardBack - 2, Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(1, boardBack - 2, Human, 4, 4, 1, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(2, boardBack - 2, Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(2, boardBack - 1, Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(2, boardBack,     Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    
    H.board.setCreature(Creature(0, boardBack - 1, Human, 2, 4, 2, Ranged, 1, 20, WEST, 0));
    H.board.setCreature(Creature(1, boardBack,     Human, 2, 4, 2, Ranged, 1, 20, WEST, 0));
    
    H.board.setCreature(Creature(1, boardBack - 1, Human, 3, 2, 1, Magic,  1, 1, WEST, 0));
    H.board.get(1, boardBack - 1).creature()->addButton("mage_strike_from_", "Fireball");
    
    //Player 2
    
    H.board.setCreature(Creature(boardBack - 2, 0, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack - 2, 1, Human, 4, 4, 1, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack - 2, 2, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack - 1, 2, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack,     2, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    
    H.board.setCreature(Creature(boardBack - 1, 0, Human, 2, 4, 2, Ranged, 1, 3, EAST, 1));
    H.board.setCreature(Creature(boardBack,     1, Human, 2, 4, 2, Ranged, 1, 3, EAST, 1));
    
    H.board.setCreature(Creature(boardBack - 1, 1, Human, 3, 2, 1, Magic,  1, 1, EAST, 1));
    H.board.get(boardBack - 1, 1).creature()->addButton("mage_strike_from_", "Fireball");
    
    /*
    H.board.setCreature(Creature(0, boardBack - 1, Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(1, boardBack - 1, Human, 4, 4, 1, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(1, boardBack,     Human, 4, 4, 3, Melee,  1, 1, WEST, 0));
    H.board.setCreature(Creature(1, boardBack - 1, Human, 4, 4, 1, Ranged, 1, 3, WEST, 0));
    
    H.board.setCreature(Creature(boardBack,     1, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack - 1, 1, Human, 4, 4, 1, Melee,  1, 3, EAST, 1));
    H.board.setCreature(Creature(boardBack - 1, 0, Human, 4, 4, 3, Melee,  1, 1, EAST, 1));
    H.board.setCreature(Creature(boardBack - 1, 1, Human, 4, 4, 1, Ranged, 1, 3, EAST, 1));
    */
    
    Building player0Home(0, BOARD_WIDTH - 1, 10, 0);
    player0Home.deathAction = "player_lose_0";
    Building player1Home(BOARD_WIDTH - 1, 0, 1, 1);
    player1Home.deathAction = "player_lose_1";
    
    H.board.setBuilding(player0Home);
    H.board.setBuilding(player1Home);
    
    H.addPlayer();
    
    H.mainClientNum = 0;
    
    //For adding players
    std::thread addPlayerThread;
    unsigned int playersToAdd = 0;
    bool runningThread = false;
    bool addedPlayer = false;
    bool failedToAddPlayer = false;
    
    //Wait for syncing with client
    while (true) {
        try {
            std::string action = H.socket.receive(0);
            if (action.find("run:") != std::string::npos) {
                action.erase(0, 4); //Erase "run:"
                if (action == "begin()") {
                    H.begin();
                    break;
                } else if (action == "addPlayer()") {
                    playersToAdd++;
                    H.socket.send("message_received", 0);
                }
            } else if (action == "send_number_of_players") {
                H.socket.send(std::to_string(H.getNumberPlayers()).c_str(), 0);
            }
        } catch (std::runtime_error) { //Keep waiting for message from client
            continue;
        }
        
        //If the thread finished running, end it, indicate no thread is running, and reset the two thread bools.
        if (addedPlayer || failedToAddPlayer) {
            addPlayerThread.join();
            runningThread = false;
            addedPlayer = false;
            failedToAddPlayer = false;
        }
        if (playersToAdd > 0 && !runningThread) { //If no adding player thread is running and a player should be added, start the thread
            playersToAdd--;
            runningThread = true;
            addPlayerThread = std::thread(threadAddPlayer, &addedPlayer, &failedToAddPlayer, &H);
        }
    }
    
    while (!*done) {
        H.update(done);
    }
}

void threadAddPlayer(bool *done, bool *failed, Host* host) {
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    while (true) {
        std::chrono::duration<double> timeElapsed = std::chrono::steady_clock::now() - start;
        if (timeElapsed.count() > MAX_CONNECTION_TIME) break;
        host->socket.setHostTimeout(2);
        try {
            host->addPlayer();
        } catch (std::runtime_error) {
            continue; //Keep trying to connect, for 10 seconds
        }
        if (done != nullptr) *done = true; //If it connected without throwing an error, mark the connection was successful and end the function
        return;
    }
    if (failed != nullptr) *failed = true;
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

//A function GLFW can call when a mouse button event occurs
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    mouseJustPressed = false;
    mouseUp = false;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseJustPressed = true; //Indicate the mouse has just been pressed. This will then be processed at the start and end of each frame to set mouseDown
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouseUp = true;
    }
}

//A function GLFW can call when the window is resized
void resizeCallback(GLFWwindow *window, int width, int height) {
    //Tell OpenGL window information
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glViewport(0.0, 0.0, viewportWidth, viewportHeight);
}
