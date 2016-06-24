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

//Personal includes
#include <myGL/shader.h>
#include <myGL/camera.h>

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



//Functions:

//A function that allows GLFW to deal with certain events like key-pressing
void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode);

//A function that allows translation of the camera
void moveCamera();



//Variables:

//Window size
const GLuint windowWidth = 800, windowHeight = 600;

//True when the game should end
bool gameOver = false;

//Array of each key, whether it is pressed or not
bool keys[1024];

//Camera object
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Previous times, for calculating camera motion at a steady rate on all systems
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main(int argc, const char * argv[]) {
    //Set up
    srand((int)time(NULL));
    
//Initialization:
    //Initiate GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Error with accidental use of legacy functions
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Non-resizable
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //On OS X
    
    //Make a window object
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "My Window", nullptr, nullptr);
    if (window == nullptr) { //If the window isn't created, return an error
        glfwTerminate();
        std::cout << "Failed to create GLFW Window.\n";
    }
    
    glfwMakeContextCurrent(window);
    
    //Initialize GLEW
    glewExperimental = GL_TRUE; //Allows the use of more modern OpenGL functionality
    if (glewInit() != GLEW_OK) { //If GLEW isn't properly initialized, return an error
        std::cout << "Failed to initialize GLEW.\n";
    }
    
    //Tell OpenGL window information
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    //Set key callback function
    glfwSetKeyCallback(window, keyCallback);
    
//Drawing:
    //Create an object wthat contains the compiled shader
    Shader shader("Shaders/shader.vert", "Shaders/shader.geom", "Shaders/shader.frag");
    
    //Make a 2D array of single points, which will each be the center of the board square
    //The geometry shader will turn a point into a square centered at that point
    //Later on we will just load all of the map data in from a file. Hardcoded for now
    GLfloat vertices[] = {
    //   position      Color
        -0.9f, -0.9f,  0.0f, 0.3f, 0.0f,
        -0.7f, -0.9f,  0.0f, 0.5f, 0.0f,
        -0.5f, -0.9f,  0.0f, 0.3f, 0.0f,
        -0.3f, -0.9f,  0.0f, 0.5f, 0.0f,
        -0.1f, -0.9f,  0.0f, 0.3f, 0.0f,
         0.1f, -0.9f,  0.0f, 0.5f, 0.0f,
         0.3f, -0.9f,  0.0f, 0.3f, 0.0f,
         0.5f, -0.9f,  0.0f, 0.5f, 0.0f,
         0.7f, -0.9f,  0.0f, 0.3f, 0.0f,
         0.9f, -0.9f,  0.0f, 0.5f, 0.0f,
        
        -0.9f, -0.7f,  0.0f, 0.5f, 0.0f,
        -0.7f, -0.7f,  0.0f, 0.3f, 0.0f,
        -0.5f, -0.7f,  0.0f, 0.5f, 0.0f,
        -0.3f, -0.7f,  0.0f, 0.3f, 0.0f,
        -0.1f, -0.7f,  0.0f, 0.5f, 0.0f,
         0.1f, -0.7f,  0.0f, 0.3f, 0.0f,
         0.3f, -0.7f,  0.0f, 0.5f, 0.0f,
         0.5f, -0.7f,  0.0f, 0.3f, 0.0f,
         0.7f, -0.7f,  0.0f, 0.5f, 0.0f,
         0.9f, -0.7f,  0.0f, 0.3f, 0.0f,
        
        -0.9f, -0.5f,  0.0f, 0.3f, 0.0f,
        -0.7f, -0.5f,  0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.3f, 0.0f,
        -0.3f, -0.5f,  0.0f, 0.5f, 0.0f,
        -0.1f, -0.5f,  0.0f, 0.3f, 0.0f,
         0.1f, -0.5f,  0.0f, 0.5f, 0.0f,
         0.3f, -0.5f,  0.0f, 0.3f, 0.0f,
         0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
         0.7f, -0.5f,  0.0f, 0.3f, 0.0f,
         0.9f, -0.5f,  0.0f, 0.5f, 0.0f,
        
        -0.9f, -0.3f,  0.0f, 0.5f, 0.0f,
        -0.7f, -0.3f,  0.0f, 0.3f, 0.0f,
        -0.5f, -0.3f,  0.0f, 0.5f, 0.0f,
        -0.3f, -0.3f,  0.0f, 0.3f, 0.0f,
        -0.1f, -0.3f,  0.0f, 0.5f, 0.0f,
         0.1f, -0.3f,  0.0f, 0.3f, 0.0f,
         0.3f, -0.3f,  0.0f, 0.5f, 0.0f,
         0.5f, -0.3f,  0.0f, 0.3f, 0.0f,
         0.7f, -0.3f,  0.0f, 0.5f, 0.0f,
         0.9f, -0.3f,  0.0f, 0.3f, 0.0f,
        
        -0.9f, -0.1f,  0.0f, 0.3f, 0.0f,
        -0.7f, -0.1f,  0.0f, 0.5f, 0.0f,
        -0.5f, -0.1f,  0.0f, 0.3f, 0.0f,
        -0.3f, -0.1f,  0.0f, 0.5f, 0.0f,
        -0.1f, -0.1f,  0.0f, 0.3f, 0.0f,
         0.1f, -0.1f,  0.0f, 0.5f, 0.0f,
         0.3f, -0.1f,  0.0f, 0.3f, 0.0f,
         0.5f, -0.1f,  0.0f, 0.5f, 0.0f,
         0.7f, -0.1f,  0.0f, 0.3f, 0.0f,
         0.9f, -0.1f,  0.0f, 0.5f, 0.0f,
        
        -0.9f,  0.1f,  0.0f, 0.5f, 0.0f,
        -0.7f,  0.1f,  0.0f, 0.3f, 0.0f,
        -0.5f,  0.1f,  0.0f, 0.5f, 0.0f,
        -0.3f,  0.1f,  0.0f, 0.3f, 0.0f,
        -0.1f,  0.1f,  0.0f, 0.5f, 0.0f,
         0.1f,  0.1f,  0.0f, 0.3f, 0.0f,
         0.3f,  0.1f,  0.0f, 0.5f, 0.0f,
         0.5f,  0.1f,  0.0f, 0.3f, 0.0f,
         0.7f,  0.1f,  0.0f, 0.5f, 0.0f,
         0.9f,  0.1f,  0.0f, 0.3f, 0.0f,
        
        -0.9f,  0.3f,  0.0f, 0.3f, 0.0f,
        -0.7f,  0.3f,  0.0f, 0.5f, 0.0f,
        -0.5f,  0.3f,  0.0f, 0.3f, 0.0f,
        -0.3f,  0.3f,  0.0f, 0.5f, 0.0f,
        -0.1f,  0.3f,  0.0f, 0.3f, 0.0f,
         0.1f,  0.3f,  0.0f, 0.5f, 0.0f,
         0.3f,  0.3f,  0.0f, 0.3f, 0.0f,
         0.5f,  0.3f,  0.0f, 0.5f, 0.0f,
         0.7f,  0.3f,  0.0f, 0.3f, 0.0f,
         0.9f,  0.3f,  0.0f, 0.5f, 0.0f,
        
        -0.9f,  0.5f,  0.0f, 0.5f, 0.0f,
        -0.7f,  0.5f,  0.0f, 0.3f, 0.0f,
        -0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
        -0.3f,  0.5f,  0.0f, 0.3f, 0.0f,
        -0.1f,  0.5f,  0.0f, 0.5f, 0.0f,
         0.1f,  0.5f,  0.0f, 0.3f, 0.0f,
         0.3f,  0.5f,  0.0f, 0.5f, 0.0f,
         0.5f,  0.5f,  0.0f, 0.3f, 0.0f,
         0.7f,  0.5f,  0.0f, 0.5f, 0.0f,
         0.9f,  0.5f,  0.0f, 0.3f, 0.0f,
        
        -0.9f,  0.7f,  0.0f, 0.3f, 0.0f,
        -0.7f,  0.7f,  0.0f, 0.5f, 0.0f,
        -0.5f,  0.7f,  0.0f, 0.3f, 0.0f,
        -0.3f,  0.7f,  0.0f, 0.5f, 0.0f,
        -0.1f,  0.7f,  0.0f, 0.3f, 0.0f,
         0.1f,  0.7f,  0.0f, 0.5f, 0.0f,
         0.3f,  0.7f,  0.0f, 0.3f, 0.0f,
         0.5f,  0.7f,  0.0f, 0.5f, 0.0f,
         0.7f,  0.7f,  0.0f, 0.3f, 0.0f,
         0.9f,  0.7f,  0.0f, 0.5f, 0.0f,
        
        -0.9f,  0.9f,  0.0f, 0.5f, 0.0f,
        -0.7f,  0.9f,  0.0f, 0.3f, 0.0f,
        -0.5f,  0.9f,  0.0f, 0.5f, 0.0f,
        -0.3f,  0.9f,  0.0f, 0.3f, 0.0f,
        -0.1f,  0.9f,  0.0f, 0.5f, 0.0f,
         0.1f,  0.9f,  0.0f, 0.3f, 0.0f,
         0.3f,  0.9f,  0.0f, 0.5f, 0.0f,
         0.5f,  0.9f,  0.0f, 0.3f, 0.0f,
         0.7f,  0.9f,  0.0f, 0.5f, 0.0f,
         0.9f,  0.9f,  0.0f, 0.3f, 0.0f,
    };
    
    //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    //First we bind the VAO
    glBindVertexArray(VAO);
    
    //Bind the other buffers with the data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Positionn
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Uncomment for wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //Game loop
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //GLFW gets any events that have occurred
        glfwPollEvents();
        
        //Move camera based on any inputs
        moveCamera();
        
        //Clears the screen after each rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Use the shader
        shader.use();
        
        //Bind the VAO and draw shapes
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 100);
        glBindVertexArray(0);
        
        //Swap buffers so as to properly render without flickering
        glfwSwapBuffers(window);
    }
    
//Termination:
    //Deallocate all any resources when we are finished
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

/**
 *A function to move the camera up, down, left, or right based on key presses.
 */
void moveCamera() {
    //Calls translation functions from the camera
    if (keys[GLFW_KEY_UP]) {
        camera.processKeyboard(FORWARD, deltaTime, false);
    }
    if (keys[GLFW_KEY_DOWN]) {
        camera.processKeyboard(BACKWARD, deltaTime, false);
    }
    if (keys[GLFW_KEY_LEFT]) {
        camera.processKeyboard(LEFT, deltaTime, false);
    }
    if (keys[GLFW_KEY_RIGHT]) {
        camera.processKeyboard(RIGHT, deltaTime, false);
    }
}

/**
 *A function GLFW can call when a key event occurs
 *
 *@param window The GLFWwindow object.
 *@param key The macro that will represent the key pressed
 *@param action The macro that represents if the key is being pressed, released, etc...
 *@param mode The macro representing which, if any, modes are activated, such as shift, command, etc...
 */
void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode) {
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