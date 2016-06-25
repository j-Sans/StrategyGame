//
//  Game.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

//Standard library includes
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
#include "GLextensions/shader.hpp"
#include "Tile.hpp"

//An array of booleans representing if, for each key, if that key is pressed
//Declared here so it can work with static function keyCallback. That function needs to be static
bool keys[1024];

class Game {
public:
    //Constructor
    Game(Shader shader, std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    const GLuint windowWidth = 800;
    const GLuint windowHeight = 600;
    
    //Public member functions
    
    //Get functions
    
private:
    //Private properties
    
    GLFWwindow* window;
    
    
    //Private member functions
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void initWindow();
};

#endif /* Game_hpp */
