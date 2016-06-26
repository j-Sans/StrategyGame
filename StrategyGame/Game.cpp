//
//  Game.cpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Game.hpp"

//An array of booleans representing if, for each key, if that key is pressed
//Declared here so it can work with static function keyCallback. That function needs to be static
bool keys[1024];

//Constructor without geometry shader
Game::Game(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setVertexData(); //Set the vertex data with an std::array
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    gameShader = Shader(vertexPath, fragmentPath);
    
    //Allow for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Load textures
    //Exception only thrown if there are 32 textures already present
    try {
        this->loadTexture("Resources/grass.jpg", "grassTex");
    } catch (std::exception e) {
        std::cout << "Error loading grass texture: " << e.what();
    }
    try {
        this->loadTexture("Resources/mountain.png", "mountainTex");
    } catch (std::exception e) {
        std::cout << "Error loading mountain texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Constructor with geometry shader
Game::Game(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setVertexData(); //Set the vertex data with an std::array
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    gameShader = Shader(vertexPath, geometryPath, fragmentPath);
    
    //Allow for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Load textures
    //Exception only thrown if there are 32 textures already present
    try {
        this->loadTexture("Resources/grass.jpg", "grassTex");
    } catch (std::exception e) {
        std::cout << "Error loading grass texture: " << e.what();
    }
    try {
        this->loadTexture("Resources/mountain.png", "mountainTex");
    } catch (std::exception e) {
        std::cout << "Error loading mountain texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Public member functions

//A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
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
    
    for (auto tex = textures.begin(); tex != textures.end(); tex++) {
        tex->use(this->gameShader);
    }
    
    //Set the camera-translation vector based on arrowkey inputs
    this->moveCamera();
    
    //Affect the camera position and send the view matrix to the shader
    view = glm::translate(view, cameraCenter);
    this->gameShader.uniformMat4("view", view);
    
    //Reset the view matrix
    this->view = glm::mat4();
    
    //Bind the VAO and draw shapes
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, 100);
    glBindVertexArray(0);
    
    //Swap buffers so as to properly render without flickering
    glfwSwapBuffers(this->gameWindow);
}

//Close the window
void Game::closeWindow() {
    glfwSetWindowShouldClose(this->gameWindow, GL_TRUE);;
}

//Terminate the window
void Game::terminate() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    
    glfwTerminate();
}

//Set the clear color of the screen.
const void Game::setClearColor(GLfloat red, GLfloat green, GLfloat blue) {
    red = red >= 1.0f ? 1.0f : red <= 0.0f ? 0.0f : red;
    green = green >= 1.0f ? 1.0f : green <= 0.0f ? 0.0f : green;
    blue = blue >= 1.0f ? 1.0f : blue <= 0.0f ? 0.0f : blue;
    this->clearColor = glm::vec3(red, green, blue);
}

//Get a pointer to the window
GLFWwindow* Game::window() {
    return this->gameWindow;
}

//Get the time since the previous frame
const GLfloat Game::timeSinceLastFrame() {
    return this->deltaTime;
}

//Private member functions

//Initialize GLFW, GLEW, the key callback function, and the window itself.
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
    this->gameWindow = glfwCreateWindow(this->windowWidth, this->windowHeight, "Game", nullptr, nullptr);
    if (this->gameWindow == nullptr) { //If the window isn't created, return an error
        glfwTerminate();
        std::cout << "Failed to create GLFW Window.\n";
    }
    
    glfwMakeContextCurrent(this->gameWindow);
    
    //Initialize GLEW
    glewExperimental = GL_TRUE; //Allows the use of more modern OpenGL functionality
    if (glewInit() != GLEW_OK) { //If GLEW isn't properly initialized, return an error
        std::cout << "Failed to initialize GLEW.\n";
    }
    
    //Tell OpenGL window information
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(this->gameWindow, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);
    
    //Set key callback function
    glfwSetKeyCallback(this->gameWindow, this->keyCallback);
}

//Set the vertex data as a std::array in the object. Eventually will be made to get the data from the board or from a file, but is hardcoded for now.
void Game::setVertexData() {
    //Eventually this function will manipulate the board to get an output, or even load in from file
    
    //Make a 2D array of single points, which will each be the center of the board square
    //The geometry shader will turn a point into a square centered at that point
    GLfloat vertices[NUMBER_OF_TILES * INDICES_PER_TILES] = {
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
        0.7f,  0.7f,  OPEN_TERRAIN,
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
    
    for (int a = 0; a < NUMBER_OF_TILES * INDICES_PER_TILES; a++) {
        this->vertexData[a] = vertices[a];
    }
}

//Initialize OpenGL buffers with the object's vertex data.
void Game::setBuffers() {
    //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the other buffers with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData), this->vertexData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Uncomment for wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//Loads a texture into the back of the vector of texture objects. Only works up to 32 times. Throws an error if there are already 32 textures.
void Game::loadTexture(const GLchar* texPath, const GLchar* texName) {
    if (textures.size() <= 32)
        textures.push_back(Texture(texPath, (GLuint)textures.size(), texName));
    else
        throw std::range_error("32 textures already loaded.");
}
    
//Replaces the designated spot in the vector of texture objects with a new texture. Throws an error if the desired index is out of vector range.
void Game::replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName) {
    if (texIndex < textures.size())
        textures[texIndex] = Texture(texPath, texIndex, texName);
    else
        throw std::range_error("No texture loaded in that spot.");
}

//Contains matrix transformations to be done on the board. This sets model and projection matrices. Called only once
void Game::presetTransformations() {
    this->gameShader.use();
    
    //Make the board appear to be tilted away by keeping width double the size of heights
    this->model = glm::scale(this->model, glm::vec3(1.0f, 0.5f, 1.0f));
    
    //Make the board rotated 45º
    this->model = glm::rotate(this->model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //Send the model matrix to the shader
    this->gameShader.uniformMat4("model", model);
    
    //Orthographic (non-3D projection) added so that different window sizes don't distort the scale
    this->projection = glm::ortho((GLfloat)windowWidth / (GLfloat)windowHeight * -1.0f, (GLfloat)windowWidth / (GLfloat)windowHeight * 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //Send the projection matrix to the shader
    this->gameShader.uniformMat4("ortho", projection);
}

//A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
void Game::moveCamera() {
    GLfloat displacement = this->deltaTime * this->camSpeed;
    
    if (keys[GLFW_KEY_DOWN]) {
        this->cameraCenter.y += displacement;
    }
    if (keys[GLFW_KEY_UP]) {
        this->cameraCenter.y -= displacement;
    }
    if (keys[GLFW_KEY_LEFT]) {
        this->cameraCenter.x += displacement;
    }
    if (keys[GLFW_KEY_RIGHT]) {
        this->cameraCenter.x -= displacement;
    }
    
    //Guarantees that the camera won't move too far from the board center
    if (this->cameraCenter.x > this->camMaxDisplacement)
        this->cameraCenter.x = this->camMaxDisplacement;
    if (this->cameraCenter.x < -this->camMaxDisplacement)
        this->cameraCenter.x = -this->camMaxDisplacement;
    if (this->cameraCenter.y > this->camMaxDisplacement)
        this->cameraCenter.y = this->camMaxDisplacement;
    if (this->cameraCenter.y < -this->camMaxDisplacement)
        this->cameraCenter.y = -this->camMaxDisplacement;
}

//A function GLFW can call when a key event occurs
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