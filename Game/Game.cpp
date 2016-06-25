//
//  Game.cpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Game.hpp"

//Constructor
Game::Game(Shader shader, std::vector<std::vector<Tile> > board) : gameShader(shader) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setVertexData(); //Set the vertex data with an std::array
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    //Allow for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Load textures
    this->loadTexture("Resources/grass.jpg", 0);
    this->loadTexture("Resources/mountain.png", 1);
    
    this->presetTransformations();
}

//Public member functions

/**
 * Set the clear color of the screen.
 *
 * @param red The red value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
 * @param green The green value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
 * @param blue The blue value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
 */
const void Game::setClearColor(GLfloat red, GLfloat green, GLfloat blue) {
    red = red >= 1.0f ? 1.0f : red <= 0.0f ? 0.0f : red;
    green = green >= 1.0f ? 1.0f : green <= 0.0f ? 0.0f : green;
    blue = blue >= 1.0f ? 1.0f : blue <= 0.0f ? 0.0f : blue;
    this->clearColor = glm::vec3(red, green, blue);
}

//Private member functions

/**
 * Initialize GLFW, GLEW, the key callback function, and the window itself.
 */
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
 * Set the vertex data as a std::array in the object. Eventually will be made to get the data from the board or from a file, but is hardcoded for now.
 */
void Game::setVertexData() {
    //Eventually this function will manipulate the board to get an output, or even load in from file
    
    //Make a 2D array of single points, which will each be the center of the board square
    //The geometry shader will turn a point into a square centered at that point
    vertexData = {
    //  position      terrain type
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
}

/**
 * Initialize OpenGL buffers with the object's vertex data.
 */
void Game::setBuffers() {
    //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the other buffers with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData.data()), this->vertexData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Positionn
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Uncomment for wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/**
 * Loads a texture into the texture array as a GLuint.
 *
 * @param texPath A string representing the path to the texture image
 * @param texNumber A GLuint representing the number the texture should be, when called by GL_TEXTURE0 or whichever number. Goes into that index in the array. Range should be 0-15. If greater, then will be treated as if it is 16.
 */
void Game::loadTexture(const GLchar* texPath, GLuint texNumber) {
    if (texNumber > 15) {
        texNumber = 15; //Stops bad access from accessing greater than element 15 in the size-16 array textures
    }
    
    textures[texNumber] = Texture(texPath, texNumber);
}

/**
 * A function that sets matrix transformations to be done on the board. This sets the model matrix to rotate the board by 45º and then make it seem tilted away from the viewer. That is done by scaling so that the horizontal diagnal is double the vertical one. The projection matrix is also added, using glm::ortho, so that a non-3D orthographic projection is achieved. It is made so that regardless of window dimensions, the scaling on the board is always constant.
 */
void Game::presetTransformations() {
    this->gameShader.use();
    
    //Make the board appear to be tilted away by keeping width double the size of heights
    this->model = glm::scale(this->model, glm::vec3(1.0f, 0.5f, 1.0f));
    
    //Make the board rotated 45º
    this->model = glm::rotate(this->model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //Send the model matrix to the shader
    gameShader.uniformMat4("model", model);
    
    //Orthographic (non-3D projection) added so that different window sizes don't distort the scale
    this->projection = glm::ortho((GLfloat)windowWidth / (GLfloat)windowHeight * -1.0f, (GLfloat)windowWidth / (GLfloat)windowHeight * 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //Send the projection matrix to the shader
    gameShader.uniformMat4("ortho", projection);
}

/**
 * A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
 */
void Game::render() {
    GLfloat currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
    //GLFW gets any events that have occurred
    glfwPollEvents();
    
    //Clears the screen after each rendering
    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Use the shader
    this->gameShader.use();
    
    
    
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