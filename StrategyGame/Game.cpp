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
Game::Game(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : gameBoard(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true); //Set all of the data arrays with information from the board
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
    try {
        this->loadTexture("Resources/stick_figure.png", "stickFigureTex");
    } catch (std::exception e) {
        std::cout << "Error loading stick figure texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Constructor with geometry shader
Game::Game(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : gameBoard(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true); //Set the data arrays with information from the board
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
    try {
        this->loadTexture("Resources/stick_figure.png", "stickFigureTex");
    } catch (std::exception e) {
        std::cout << "Error loading stick figure texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Public member functions

GLfloat Game::getDistance(glm::vec2 point1, glm::vec2 point2) {
    return sqrtf(powf(point1.x - point2.x, 2.0) + powf(point1.y - point2.y, 2.0));
}

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
    
    //Update the creatures
    this->updateCreatureBuffer();
    
    //Update tile colors
    this->updateColorBuffer();
    
    //Set the camera-translation vector based on arrowkey inputs
    this->moveCamera();
    
    //Affect the camera position and send the view matrix to the shader
    view = glm::translate(view, cameraCenter);
    this->gameShader.uniformMat4("view", view);
    
    //Reset the view matrix
    this->view = glm::mat4();
    
    //Bind the VAO and draw shapes
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, NUMBER_OF_TILES);
    glBindVertexArray(0);
    
    //Swap buffers so as to properly render without flickering
    glfwSwapBuffers(this->gameWindow);
}

//Close the window
void Game::closeWindow() {
    glfwSetWindowShouldClose(this->gameWindow, GL_TRUE);
}

//Terminate the window
void Game::terminate() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->vertexVBO);
    glDeleteBuffers(1, &this->terrainVBO);
    glDeleteBuffers(1, &this->creatureVBO);
    
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

//Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
void Game::setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData) {
    //Distance between each seed point
    GLfloat pointDistance = 0.2f;
    
    GLfloat locationOfFirstPoint = 0.0f;
    locationOfFirstPoint += (this->gameBoard.width() * pointDistance / 2.0f); //Sets the board halfway behind 0 and halfway in front
    locationOfFirstPoint += (pointDistance / 2.0f); //Otherwise the 0.2 distance would be after each point (as if they were right-aligned). Instead they are center-aligned essentially.
    
    //Vertex data
    GLuint numberOfIndices = NUMBER_OF_TILES * INDICES_PER_TILES;
    
    GLuint index = 0;
    
    if (setVertexData) {
        GLfloat vertices[numberOfIndices];
        
        for (GLuint x = 0; x < this->gameBoard.width(); x++) {
            for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                if (index + 1 < numberOfIndices) { //Plus 1 because it is checked twice, so it will be incrimented twice. Checks to make sure no data outside of the array is accessed.
                    
                    //Sets the point location based on the location in the board and on the modifiers above.
                    vertices[index] = locationOfFirstPoint - (x * pointDistance);
                    index++;
                    
                    vertices[index] = locationOfFirstPoint - (y * pointDistance);
                    index++;
                }
            }
        }
        
        for (int a = 0; a < NUMBER_OF_TILES * INDICES_PER_TILES; a++) {
            this->vertexData[a] = vertices[a];
        }
    }
    
    //Terrain and creature data. One for each tile
    numberOfIndices = NUMBER_OF_TILES;
    
    GLint terrains[numberOfIndices];
    GLint creatures[numberOfIndices];
    GLint colors[3 * numberOfIndices];
    
    index = 0;
    
    for (GLuint x = 0; x < this->gameBoard.width(); x++) {
        for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
            if (index < numberOfIndices) { //Checks to make sure no data outside of the array is accessed.
                if (setTerrainData)
                    //Gets the terrain of the tile
                    terrains[index] = this->gameBoard.get(x, y).terrain();
                
                if (setCreatureData)
                    //Gets the creature on the tile
                    creatures[index] = this->gameBoard.get(x, y).creatureType();
                
                if (setColorData) {
                    //Gets the color alteration of the tile
                    colors[3 * index] = this->gameBoard.get(x, y).color().x;
                    colors[(3 * index) + 1] = this->gameBoard.get(x, y).color().y;
                    colors[(3 * index) + 2] = this->gameBoard.get(x, y).color().z;
                }
                
                //Increment
                index++;
            }
        }
    }
    
    for (int a = 0; a < NUMBER_OF_TILES; a++) {
        if (setTerrainData)
            this->terrainData[a] = terrains[a];
        if (setCreatureData)
            this->creatureData[a] = creatures[a];
        if (setColorData) {
            this->colorData[3 * a] = colors[3 * a];
            this->colorData[(3 * a) + 1] = colors[(3 * a) + 1];
            this->colorData[(3 * a) + 2] = colors[(3 * a) + 2];
        }
    }

}

//Initialize OpenGL buffers with the object's vertex data.
void Game::setBuffers() {
    //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->vertexVBO);
    glGenBuffers(1, &this->terrainVBO);
    glGenBuffers(1, &this->creatureVBO);
    glGenBuffers(1, &this->colorVBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Vertex VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData), this->vertexData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, INDICES_PER_TILES * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Terrain VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->terrainData), this->terrainData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Creature VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Color VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData), this->colorData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
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
    this->gameShader.uniformMat4("model", this->model);
    
    //Orthographic (non-3D projection) added so that different window sizes don't distort the scale
    this->projection = glm::ortho((GLfloat)windowWidth / (GLfloat)windowHeight * -1.0f, (GLfloat)windowWidth / (GLfloat)windowHeight * 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //Send the projection matrix to the shader
    this->gameShader.uniformMat4("ortho", this->projection);
    
    //Make the board appear to be tilted away by keeping width double the size of heights
    this->creatureMat = glm::scale(this->creatureMat, glm::vec3(0.5f, 0.5f, 1.0f));
    
    //Make the creature appear with the tile it's at
    this->creatureMat = glm::rotate(this->creatureMat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    this->gameShader.uniformMat4("creatureMat", this->creatureMat);
}

//A function to update the creature VBO. Should be called every frame
void Game::updateCreatureBuffer() {
    //Update creature data array
    this->setData(false, false, true, false);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

//A function to update the creature VBO. Should be called every frame
void Game::updateColorBuffer() {
    //Update creature data array
    this->setData(false, false, false, true);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData), this->colorData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
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

GLint Game::calculateTile() {
    GLint tileIndex = -1; //The tile index where the mouse was clicked. Initialized as -1 to mean no index found
    
    int width, height;
    double xPos, yPos;
    
    glfwGetCursorPos(this->gameWindow, &xPos, &yPos);
    
    glfwGetWindowSize(this->gameWindow, &width, &height);
    
    //Get the mousePos to be in the same coordinates as the vertexData (-1 to 1)
    
    //Make mousePos between 0 and 1 by dividing the position by the maximum position (width or height)
    xPos /= width;
    yPos /= height;
    
    //Now make it 0 to 2 by doubling it
    xPos *= 2.0f;
    yPos *= 2.0f;
    
    //Now subtract 1 to get it between -1 and 1
    xPos -= 1.0f;
    yPos -= 1.0f;
    
    glm::vec4 mousePos(xPos, yPos, 0.0f, 1.0f);
    
    mousePos = this->projection * this->view * this->model * mousePos;
    
    glm::vec4 tileCenters[NUMBER_OF_TILES]; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
    }
    
    //The distance from one point to the horizontal point and the vertical point:
    
    //The points diagonally above and below each vertex become horizontal and vertical after rotation. To find them, find the point below the vertex and add one and subtract one.
    
    if (BOARD_WIDTH * BOARD_WIDTH < BOARD_WIDTH + 1) { //In case finding the distances (just below) would cause a bad access
        throw std::length_error("Board too small");
    }
    
    GLfloat distance1 = Game::getDistance(tileCenters[0], tileCenters[0 + BOARD_WIDTH + 1]); //Diagonal down and to the right
    GLfloat distance2 = Game::getDistance(tileCenters[1], tileCenters[1 + BOARD_WIDTH - 1]); //Diagonal down and to the left
    
    //Distance horizontally is double the distance of the vertical one because it was compressed vertically.
    //The horizontal distance is the max of the above distances, and the vertical distance the minimum
    
    GLfloat horizontalDistance = fmaxf(distance1, distance2);
    GLfloat verticalDistance = fminf(distance1, distance2);
    
    //For every point, check if it is within the boundaries of the respective diamond's bounds, by finding the 4 bounding lines of that rectange
    
    GLfloat slope = verticalDistance / horizontalDistance; // = rise / run
    
    //Using line equation:
    // y = slope ( x - h ) + k
    //Where (h,k) is a point on the line
    
    for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
        glm::vec2 center = glm::vec2(tileCenters[index].x, tileCenters[index].y);
        
        bool pointInIndex = true;
        
        //Lower left inequality:
        // y > ( -slope ) ( x - h ) + k
        // (h,k) is the point below the center
        
        GLfloat h = center.x;
        GLfloat k = center.y - verticalDistance;
        
        if (mousePos.y < ( -slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
        }
        
        //Lower right inequality:
        // y > ( slope ) ( x - h ) + k
        // (h,k) is the point below the center, the same as previously
        
        if (mousePos.y < ( slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
        }
        
        //Upper left inequality:
        // y < ( slope ) ( x - h ) + k
        // (h,k) is the point above the center
        
        h = center.x; //h stays the same
        k = center.y + verticalDistance;
        
        if (mousePos.y > ( slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
        }
        //Upper right inequality:
        // y < ( -slope ) ( x - h ) + k
        // (h,k) is the point above the center, the same as previously
        
        if (mousePos.y > ( -slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
        }
        
        if (pointInIndex) { //The point was in bounds
            tileIndex = index;
            break; //Point found, no need to search more
        }
    }
    
    //If no tile was found, -1 is returned. Otherwise, the index pointing to the coordinate in the array of glm::vec2's is returned
    //Since there are double the number of coordinates, this coordinate times 2 is the first coordinate of the tile in vertexData
    
    return tileIndex;
}

//A function GLFW can call when a key event occurs
void Game::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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
void Game::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        
        
        
        

    
    
    
    
    }
    
    
}