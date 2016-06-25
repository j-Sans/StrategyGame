//
//  Game.cpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Game.hpp"

//Constructor
Game::Game(Shader shader, std::vector<std::vector<Tile> > board) {
    initWindow();
}

//Public member functions

//Private member functions
void Game::initWindow() {
    //Initiate GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Error with accidental use of legacy functions
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Non-resizable
#ifndef _win32
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //On OS X
#endif
    
    //Make a window object
    this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, "Game", nullptr, nullptr);
    if (this->window == nullptr) { //If the window isn't created, return an error
        glfwTerminate();
        std::cout << "Failed to create GLFW Window.\n";
    }
    
    glfwMakeContextCurrent(this->window);
    
    //Initialize GLEW
    glewExperimental = GL_TRUE; //Allows the use of more modern OpenGL functionality
    if (glewInit() != GLEW_OK) { //If GLEW isn't properly initialized, return an error
        std::cout << "Failed to initialize GLEW.\n";
    }
    
    //Tell OpenGL window information
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(this->window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    //Set key callback function    
    glfwSetKeyCallback(this->window, this->keyCallback);
}

/**
 * A function GLFW can call when a key event occurs
 *
 * @param window The GLFWwindow object.
 * @param key The macro that will represent the key pressed
 * @param action The macro that represents if the key is being pressed, released, etc...
 * @param mode The macro representing which, if any, modes are activated, such as shift, command, etc...
 */
void Game::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS && mode == GLFW_MOD_SUPER) { //Command-W: close the application
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