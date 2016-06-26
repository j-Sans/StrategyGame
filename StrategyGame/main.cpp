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


//Functions:

//A function that allows GLFW to deal with certain events like key-pressing
void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode);

//A function to be called that adjusts the camera position on arowkey clicks
void moveCamera(GLfloat deltaTime);

//A function that takes the game board and returns an array of the vertices
GLfloat* getVertices(const std::vector<std::vector<Tile> >& board);


//Variables:

//Window size
//const GLuint windowWidth = 800, windowHeight = 600;

//Board speed
const GLuint boardWidth = 10;

//True when the game should end
bool gameOver = false;

//Array of each key, whether it is pressed or not
//bool keys[1024];

//Previous times, for calculating motion at a steady rate on all systems
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//view matrix for moving the camera around
glm::vec3 cameraCenter;


int main(int argc, const char * argv[]) {
    //Set up
    srand((int)time(NULL));
    
//Gameboard:
    std::vector<std::vector<Tile> > board;
    for (GLuint a = 0; a < boardWidth; a++) {
        std::vector<Tile> row;
        for (GLuint b = 0; b < boardWidth; b++) {
            row.push_back(Tile(Open, a, b));
        }
        board.push_back(row);
    }
    
    Game G("Shaders/board.vert", "Shaders/board.geom", "Shaders/board.frag", board);
    
    /*
//Initialization:
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
    Shader shader("Shaders/board.vert", "Shaders/board.geom", "Shaders/board.frag");
    
    //Make a 2D array of single points, which will each be the center of the board square
    //The geometry shader will turn a point into a square centered at that point
    //Later on we will just load all of the map data in from a file. Hardcoded for now
    GLfloat vertices[] = {
    //   position      terrain type
         0.9f,  0.9f,  MOUNTAIN_TERRAIN,
         0.9f,  0.7f,  MOUNTAIN_TERRAIN,
         0.9f,  0.5f,  MOUNTAIN_TERRAIN,
         0.9f,  0.3f,  OPEN_TERRAIN,
         0.9f,  0.1f,  OPEN_TERRAIN,
         0.9f, -0.1f,  OPEN_TERRAIN,
         0.9f, -0.3f,  OPEN_TERRAIN,
         0.9f, -0.5f,  OPEN_TERRAIN,
         0.9f, -0.7f,  OPEN_TERRAIN,
         0.9f, -0.9f,  OPEN_TERRAIN,
        
         0.7f,  0.9f,  MOUNTAIN_TERRAIN,
         0.7f,  0.7f,  MOUNTAIN_TERRAIN,
         0.7f,  0.5f,  OPEN_TERRAIN,
         0.7f,  0.3f,  OPEN_TERRAIN,
         0.7f,  0.1f,  OPEN_TERRAIN,
         0.7f, -0.1f,  OPEN_TERRAIN,
         0.7f, -0.3f,  OPEN_TERRAIN,
         0.7f, -0.5f,  OPEN_TERRAIN,
         0.7f, -0.7f,  OPEN_TERRAIN,
         0.7f, -0.9f,  OPEN_TERRAIN,
        
         0.5f,  0.9f,  MOUNTAIN_TERRAIN,
         0.5f,  0.7f,  OPEN_TERRAIN,
         0.5f,  0.5f,  OPEN_TERRAIN,
         0.5f,  0.3f,  OPEN_TERRAIN,
         0.5f,  0.1f,  OPEN_TERRAIN,
         0.5f, -0.1f,  OPEN_TERRAIN,
         0.5f, -0.3f,  OPEN_TERRAIN,
         0.5f, -0.5f,  OPEN_TERRAIN,
         0.5f, -0.7f,  OPEN_TERRAIN,
         0.5f, -0.9f,  OPEN_TERRAIN,
        
         0.3f,  0.9f,  MOUNTAIN_TERRAIN,
         0.3f,  0.7f,  OPEN_TERRAIN,
         0.3f,  0.5f,  OPEN_TERRAIN,
         0.3f,  0.3f,  OPEN_TERRAIN,
         0.3f,  0.1f,  OPEN_TERRAIN,
         0.3f, -0.1f,  OPEN_TERRAIN,
         0.3f, -0.3f,  OPEN_TERRAIN,
         0.3f, -0.5f,  OPEN_TERRAIN,
         0.3f, -0.7f,  OPEN_TERRAIN,
         0.3f, -0.9f,  OPEN_TERRAIN,
        
         0.1f,  0.9f,  MOUNTAIN_TERRAIN,
         0.1f,  0.7f,  OPEN_TERRAIN,
         0.1f,  0.5f,  OPEN_TERRAIN,
         0.1f,  0.3f,  OPEN_TERRAIN,
         0.1f,  0.1f,  OPEN_TERRAIN,
         0.1f, -0.1f,  OPEN_TERRAIN,
         0.1f, -0.3f,  OPEN_TERRAIN,
         0.1f, -0.5f,  OPEN_TERRAIN,
         0.1f, -0.7f,  OPEN_TERRAIN,
         0.1f, -0.9f,  OPEN_TERRAIN,
        
        -0.1f,  0.9f,  OPEN_TERRAIN,
        -0.1f,  0.7f,  OPEN_TERRAIN,
        -0.1f,  0.5f,  OPEN_TERRAIN,
        -0.1f,  0.3f,  OPEN_TERRAIN,
        -0.1f,  0.1f,  OPEN_TERRAIN,
        -0.1f, -0.1f,  OPEN_TERRAIN,
        -0.1f, -0.3f,  OPEN_TERRAIN,
        -0.1f, -0.5f,  OPEN_TERRAIN,
        -0.1f, -0.7f,  OPEN_TERRAIN,
        -0.1f, -0.9f,  OPEN_TERRAIN,
        
        -0.3f,  0.9f,  OPEN_TERRAIN,
        -0.3f,  0.7f,  OPEN_TERRAIN,
        -0.3f,  0.5f,  OPEN_TERRAIN,
        -0.3f,  0.3f,  OPEN_TERRAIN,
        -0.3f,  0.1f,  OPEN_TERRAIN,
        -0.3f, -0.1f,  OPEN_TERRAIN,
        -0.3f, -0.3f,  OPEN_TERRAIN,
        -0.3f, -0.5f,  OPEN_TERRAIN,
        -0.3f, -0.7f,  OPEN_TERRAIN,
        -0.3f, -0.9f,  OPEN_TERRAIN,
        
        -0.5f,  0.9f,  OPEN_TERRAIN,
        -0.5f,  0.7f,  OPEN_TERRAIN,
        -0.5f,  0.5f,  OPEN_TERRAIN,
        -0.5f,  0.3f,  OPEN_TERRAIN,
        -0.5f,  0.1f,  OPEN_TERRAIN,
        -0.5f, -0.1f,  OPEN_TERRAIN,
        -0.5f, -0.3f,  OPEN_TERRAIN,
        -0.5f, -0.5f,  OPEN_TERRAIN,
        -0.5f, -0.7f,  OPEN_TERRAIN,
        -0.5f, -0.9f,  OPEN_TERRAIN,
        
        -0.7f,  0.9f,  OPEN_TERRAIN,
        -0.7f,  0.7f,  OPEN_TERRAIN,
        -0.7f,  0.5f,  OPEN_TERRAIN,
        -0.7f,  0.3f,  OPEN_TERRAIN,
        -0.7f,  0.1f,  OPEN_TERRAIN,
        -0.7f, -0.1f,  OPEN_TERRAIN,
        -0.7f, -0.3f,  OPEN_TERRAIN,
        -0.7f, -0.5f,  OPEN_TERRAIN,
        -0.7f, -0.7f,  OPEN_TERRAIN,
        -0.7f, -0.9f,  OPEN_TERRAIN,
        
        -0.9f,  0.9f,  OPEN_TERRAIN,
        -0.9f,  0.7f,  OPEN_TERRAIN,
        -0.9f,  0.5f,  OPEN_TERRAIN,
        -0.9f,  0.3f,  OPEN_TERRAIN,
        -0.9f,  0.1f,  OPEN_TERRAIN,
        -0.9f, -0.1f,  OPEN_TERRAIN,
        -0.9f, -0.3f,  OPEN_TERRAIN,
        -0.9f, -0.5f,  OPEN_TERRAIN,
        -0.9f, -0.7f,  OPEN_TERRAIN,
        -0.9f, -0.9f,  OPEN_TERRAIN,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Uncomment for wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //Allow for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
//Grass texture:
    int textureWidth, textureHeight;
    unsigned char *image;
    
    //Load in the image for the container texture
    image = SOIL_load_image("Resources/grass.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    
    //Make the grass texture
    GLuint grassTex;
    glGenTextures(1, &grassTex);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Generate the image for the currently bound texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //Free the memory associated with the texture and unbind it
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
//Mountain texture:
    //Load in the image for the container texture
    image = SOIL_load_image("Resources/mountain.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
    
    //Make the mountain texture
    GLuint mountainTex;
    glGenTextures(1, &mountainTex);
    glBindTexture(GL_TEXTURE_2D, mountainTex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Generate the image for the currently bound texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //Free the memory associated with the texture and unbind it
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 1);
    
//Shader matrix:
    //Send model rotation matrix to the shader
    shader.use();
    
    glm::mat4 model;
    
    //Make the board appear to be tilted away by keeping width double the size of heights
    model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
    
    //Make the board rotated 45º
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //Send the model matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    //View matrix
    glm::mat4 view;
    
    //Orthographic (non-3D projection) added so that different window sizes don't distort the scale
    glm::mat4 ortho;
    ortho = glm::ortho((GLfloat)windowWidth / (GLfloat)windowHeight * -1.0f, (GLfloat)windowWidth / (GLfloat)windowHeight * 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //Send the model matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
    */
    //Game loop
    while(!glfwWindowShouldClose(G.window())) {
        /*
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //GLFW gets any events that have occurred
        glfwPollEvents();
        
        //Clears the screen after each rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Use the shader
        shader.use();
        
        //Set the texture
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTex);
        glUniform1i(glGetUniformLocation(shader.program, "grassTex"), 0);
        
        //Set the texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mountainTex);
        glUniform1i(glGetUniformLocation(shader.program, "mountainTex"), 1);
        
        //Set the camera-translation vector based on arrowkey inputs
        moveCamera(deltaTime);
        
        //Affect the camera position and send the view matrix to the shader
        view = glm::translate(view, cameraCenter);
        glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        //Reset the view matrix
        view = glm::mat4();
        
        //Bind the VAO and draw shapes
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 100);
        glBindVertexArray(0);
        
        //Swap buffers so as to properly render without flickering
        glfwSwapBuffers(window);
         */
        G.render();
    }
    
    G.terminate();

//Termination:
    //Deallocate all any resources when we are finished
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    
//    glfwTerminate();
    return 0;
}

/** 
 * A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
 *
 * @param deltaTime A GLfloat that represents the difference in time since the last call. Ensures that on all systems the camera moves at the same speed.
 */
//void moveCamera(GLfloat deltaTime) {
//    GLfloat displacement = deltaTime * camSpeed;
//    
//    if (keys[GLFW_KEY_DOWN]) {
//        cameraCenter.y += displacement;
//    }
//    if (keys[GLFW_KEY_UP]) {
//        cameraCenter.y -= displacement;
//    }
//    if (keys[GLFW_KEY_LEFT]) {
//        cameraCenter.x += displacement;
//    }
//    if (keys[GLFW_KEY_RIGHT]) {
//        cameraCenter.x -= displacement;
//    }
//    
//    //Guaruntees that the camera won't move too far from the board center
//    if (cameraCenter.x > camMaxDisplacement)
//        cameraCenter.x = camMaxDisplacement;
//    if (cameraCenter.x < -camMaxDisplacement)
//        cameraCenter.x = -camMaxDisplacement;
//    if (cameraCenter.y > camMaxDisplacement)
//        cameraCenter.y = camMaxDisplacement;
//    if (cameraCenter.y < -camMaxDisplacement)
//        cameraCenter.y = -camMaxDisplacement;
//}

/**
 * A function GLFW can call when a key event occurs
 *
 * @param window The GLFWwindow object.
 * @param key The macro that will represent the key pressed
 * @param action The macro that represents if the key is being pressed, released, etc...
 * @param mode The macro representing which, if any, modes are activated, such as shift, command, etc...
 */
//void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
//    if (key == GLFW_KEY_W && action == GLFW_PRESS && mode == GLFW_MOD_SUPER) { //Command-W: close the application
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//    if (key >= 0 && key < 1024) {
//        if (action == GLFW_PRESS) {
//            keys[key] = true;
//        }
//        if (action == GLFW_RELEASE) {
//            keys[key] = false;
//        }
//    }
//}
//
/**
 * A function that takes the game board and returns an array of the vertices
 *
 * @param board A 2D vector of Tile objects that store the board information.
 *
 * @return The pointer to an array of vertices that can directly be sent to the VBO. For each point, the array will contain the x and y positions from -1.0 to 1.0, along with a float representing the terrain type of the tile.
 */
//GLfloat* getVertices(const std::vector<std::vector<Tile> >& board) {
//    // TO BE FINISHED
//}