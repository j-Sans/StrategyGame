//
//  Visualizer.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/20/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Visualizer.hpp"

//An array of booleans representing if, for each key, if that key is pressed
//Declared here so it can work with static function keyCallback. That function needs to be static
bool keys[1024];

//A boolean representing if the mouse has been clicked, for use in buttons and setting active tiles. This boolean is set in the mouse button callback function
bool mouseDown = false;

//A boolean representing if the mouse button has been released, for use with resetting buttons. This boolean is set in the mouse button callback function
bool mouseUp = false;

//A boolean representing if the escape button has been clicked, for use with the settings menu. This boolean is set in the key callback function and is reset when used.
bool escClicked = false;

//Only so that Client.hpp can have a shader property without declaring it initially. No other purpose
//Visualizer::Visualizer() {}

//Constructor
Visualizer::Visualizer(std::string vertexPath, std::string geometryPath, std::string fragmentPath) {
    
    this->initWindow(); //Create the GLFW window and set the window property
//    this->setData(); //Set the data arrays with information from the board
    
    this->camMaxDisplacement = this->boardWidth / 10.0f;
    
    glm::ivec2 windowSize;
    glm::ivec2 framebufferSize;
    
    glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
    glfwGetFramebufferSize(this->gameWindow, &framebufferSize.x, &framebufferSize.y);
    
    this->gameShader = Shader(vertexPath.c_str(), geometryPath.c_str(), fragmentPath.c_str());
    
    this->font = Font(FONT_PATH);
    
    this->setInterface();
    this->updateInterfaces();
    
    //Allow for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Allow for multiple windows
    glEnable(GL_SCISSOR_TEST);
    
    //Load textures
    //Exception only thrown if there are 32 textures already present
    try {
        this->loadTexture("Resources/grass.jpg", "grassTex");
    } catch (std::exception e) {
        std::cout << "Error loading grass texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/mountain.png", "mountainTex");
    } catch (std::exception e) {
        std::cout << "Error loading mountain texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/forest.png", "forestTex");
    } catch (std::exception e) {
        std::cout << "Error loading forest texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/stick_figure.png", "stickFigureTex");
    } catch (std::exception e) {
        std::cout << "Error loading stick figure texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/ScoutPlaceholder.png", "scoutPTex");
    } catch (std::exception e) {
        std::cout << "Error loading scout placeholder texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/ArcherPlaceholder.png", "archerPTex");
    } catch (std::exception e) {
        std::cout << "Error loading archer placeholder texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/tower.png", "towerTex");
    } catch (std::exception e) {
        std::cout << "Error loading numbers texture: " << e.what() << std::endl;
    }
    try {
        this->loadTexture("Resources/circle.png", "circleTex");
    } catch (std::exception e) {
        std::cout << "Error loading circle texture: " << e.what() << std::endl;
    }
    
    this->presetTransformations();
}

//Public member functions

GLfloat Visualizer::getDistance(glm::vec2 point1, glm::vec2 point2) {
    return sqrtf(powf(point1.x - point2.x, 2.0) + powf(point1.y - point2.y, 2.0));
}

void Visualizer::set(unsigned int width, unsigned int height, std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec) {
    this->boardWidth = width;
    this->boardHeight = height;
    
    this->setBuffers(terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
    
//    this->setBuffers(initialInfo); //Set up all of the OpenGL buffers with the vertex data
    
    this->isSet = true;
}

//A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
//void Visualizer::render(std::map<BoardInfoDataTypes, std::string> boardInfo) {
void Visualizer::render(std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec) {
    //*****HOW MUCH OF CLIENT INFO SECTION HERE IS BEING USED?*****
    std::string clientInfo;
    
    glm::dvec2 mousePos;
    glm::ivec2 windowSize;
    
    glfwGetCursorPos(this->gameWindow, &mousePos.x, &mousePos.y);
    glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
    
    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
    }
    
    glm::ivec2 mouseTile = this->mouseTile(mousePos, windowSize, tileCenters);
    
    clientInfo.push_back(mouseTile.x);
    clientInfo.push_back(mouseTile.y);
    clientInfo.push_back(mouseDown ? 1 : 0);
    
    //Set the selected tile if the mouse is pressing
    if (mouseDown)
        this->selectedTile = mouseTile;
    else
        this->selectedTile = NO_SELECTION;

//    this->updateBuffers(boardInfo);
    this->updateBuffers(terrainDataVec, creatureDataVec, colorDataVec, damageDataVec, offsetDataVec, buildingDataVec);
    
    this->updateInterfaces();
    
    GLfloat currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
    //GLFW gets any events that have occurred
    glfwPollEvents();
    
    //So the whole screen is cleared
    glDisable(GL_SCISSOR_TEST);
    
    //Clears the screen after each rendering
    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //So multiple windows exist again
    glEnable(GL_SCISSOR_TEST);
    
    //Use the shader
    this->gameShader.use();
    
    //Prepare all of the textures
    for (auto tex = textures.begin(); tex != textures.end(); tex++) {
        tex->use(this->gameShader);
    }
    
//    if (!this->showSettings)
//        this->game.updateSelected(&mouseDown, cursorPos, windowSize, tileCenters);
//    
//    this->updateInterfaces();
//    
//    //If the mouse clicks outside of the settings menu when it's open, close the menu
//    if (this->showSettings) {
//        
//        glm::ivec2 framebufferSize;
//        
//        glfwGetFramebufferSize(this->gameWindow, &framebufferSize.x, &framebufferSize.y);
//        
//        glm::vec2 cursorPosFramebufferCoords = cursorPos * (glm::dvec2)framebufferSize / (glm::dvec2)windowSize;
//        
//        if (cursorPosFramebufferCoords.x < this->settingsMenuStats.x || cursorPosFramebufferCoords.x > this->settingsMenuStats.x + this->settingsMenuStats.width || cursorPosFramebufferCoords.y < this->settingsMenuStats.y || cursorPosFramebufferCoords.y > this->settingsMenuStats.y + this->settingsMenuStats.height) {
//            this->showSettings = false;
//        }
//    }
//    
//    if (escClicked) { //When escape is clicked change whether the settings menu is shown or not
//        this->showSettings = !this->showSettings;
//        escClicked = false;
//    }
    
    this->moveCamera();
    
    //Reset the view matrix
    this->view = glm::mat4();
    
    //Affect the camera position and send the view matrix to the shader
    this->view = glm::translate(this->view, cameraCenter);
    this->gameShader.uniformMat4("view", this->view);
    
    //Bind the VAO and draw shapes
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, this->numberOfTiles);
    glBindVertexArray(0);
    
    this->renderDamageText();
    
    //Render the left, bottom, and right interfaces
    
    for (int a = 0; a < 3; a++) {
        Interface *interface;
        
        if (a == 0)
            interface = this->leftInterface;
        else if (a == 1)
            interface = this->bottomInterface;
        else if (a == 2)
            interface = this->rightInterface;
        
        //This renders the interface and its buttons
        interface->render(mouseDown, mouseUp, !this->showSettings);
        
        //Go through the buttons and check if they are pressed, and do any consequential actions
        for (auto button = interface->buttons.begin(); button != interface->buttons.end(); button++) {
            if (button->isPressed()) {
                this->processButton(button->action);
            }
        }
    }
    
//    if (this->showSettings)
//        this->renderSettingsMenu(mouseUp, mouseDown);
    
    //mouseDown is likely set to false above, but not if the mouse was clicked in an interface box. In that case, the above for loop deals with it, and now it is no longer needed to be true, so it is reset
    if (mouseDown)
        mouseDown = false;
    
    //Swap buffers so as to properly render without flickering
    glfwSwapBuffers(this->gameWindow);
}

std::string Visualizer::getClientInfo() {
    glm::dvec2 mousePos;
    glm::ivec2 windowSize;
    
    glfwGetCursorPos(this->gameWindow, &mousePos.x, &mousePos.y);
    glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
    
    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
    }
    
    glm::ivec2 mouseTile = this->mouseTile(mousePos, windowSize, tileCenters);
    
    return std::to_string(mouseTile.x) + ',' + std::to_string(mouseTile.y) + ',' + (mouseDown ? '1' : '0');
}
    
    
    
    
    
    /*
    
    //If the mouse was clicked, set the color of the tile that was clicked
    if (mouseDown) {
        //Set mouseDown to false because this next function deals with the mouse and updates accordingly.
        mouseDown = false;
        
        glm::dvec2 cursorPos;
        glm::ivec2 windowSize;
        
        glfwGetCursorPos(this->gameWindow, &cursorPos.x, &cursorPos.y);
        glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
        
        glm::vec4 tileCenters[this->numberOfTiles]; //Representing the center point of all of the map squares
        
        for (GLuint index = 0; index < this->numberOfTiles; index++) {
            //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
            tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
        }
    
        //This function deals with mouse clicks. If the mouse was clicked in an interface box, mouseDown is returned to true so that the buttons can check if there is any click. This only updates if the settings menu is not up.
        
    }
    
    this->game.updateCreatures(this->deltaTime);
    
    //Update the buffers that need updating.
    this->updateBuffers();
    
    this->updateInterfaces();
    
    //Set the camera-translation vector based on arrowkey inputs
   */

//Close the window
void Visualizer::closeWindow() {
    glfwSetWindowShouldClose(this->gameWindow, GL_TRUE);
}

//Terminate the window
void Visualizer::terminate() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->vertexVBO);
    glDeleteBuffers(1, &this->terrainVBO);
    glDeleteBuffers(1, &this->creatureVBO);
    glDeleteBuffers(1, &this->colorVBO);
    glDeleteBuffers(1, &this->damageVBO);
    glDeleteBuffers(1, &this->offsetVBO);
    glDeleteBuffers(1, &this->buildingVBO);
    
    glfwTerminate();
}

//Set the clear color of the screen.
const void Visualizer::setClearColor(GLfloat red, GLfloat green, GLfloat blue) {
    red = red >= 1.0f ? 1.0f : red <= 0.0f ? 0.0f : red;
    green = green >= 1.0f ? 1.0f : green <= 0.0f ? 0.0f : green;
    blue = blue >= 1.0f ? 1.0f : blue <= 0.0f ? 0.0f : blue;
    this->clearColor = glm::vec3(red, green, blue);
}

//Get a pointer to the window
GLFWwindow* Visualizer::window() {
    return this->gameWindow;
}

//Get the time since the previous frame
const GLfloat Visualizer::timeSinceLastFrame() {
    return this->deltaTime;
}

//Private member functions

//Initialize GLFW, GLEW, the key callback function, and the window itself.
void Visualizer::initWindow() {
    //Initiate GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Error with accidental use of legacy functions
    
#ifdef RESIZEABLE
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
#ifndef RESIZEABLE
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif
    
#ifndef _win32
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //On OS X
#endif
    
    //Make a window object
    this->gameWindow = glfwCreateWindow(this->windowWidth, this->windowHeight, "Game", nullptr, nullptr);
    
#ifdef FULL_SCREEN //Makes the window take up the entire screen but still be within a moveable window
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    glfwSetWindowMonitor(this->gameWindow, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
#endif
    
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
    glViewport(this->leftInterfaceStats.width, this->bottomInterfaceStats.height, viewportWidth - this->leftInterfaceStats.width - this->rightInterfaceStats.width, viewportHeight - this->bottomInterfaceStats.height); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfacecs
    
    this->viewportSize = glm::ivec2(viewportWidth - this->leftInterfaceStats.width - this->rightInterfaceStats.width, viewportHeight - this->bottomInterfaceStats.height);
    
    //Set key callback function
    glfwSetKeyCallback(this->gameWindow, this->keyCallback);
    
    //Set mouse button click callback function
    glfwSetMouseButtonCallback(this->gameWindow, this->mouseButtonCallback);
}

////Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
//void Visualizer::setData() {
//    //Distance between each seed point
//    GLfloat pointDistance = 0.2f;
//    
//    GLfloat locationOfFirstPoint = 0.0f;
//    locationOfFirstPoint += (this->boardWidth * pointDistance / 2.0f); //Sets the board halfway behind 0 and halfway in front
//    locationOfFirstPoint += (pointDistance / 2.0f); //Otherwise the 0.2 distance would be after each point (as if they were right-aligned). Instead they are center-aligned essentially.
//    
//    //Vertex data
//    GLuint numberOfIndices = this->numberOfTiles * INDICES_PER_TILES;
//    
//    GLuint index = 0;
//    
//    GLfloat vertices[numberOfIndices];
//    
//    for (GLuint x = 0; x < this->boardWidth; x++) {
//        for (GLuint y = 0; y < this->boardHeight; y++) {
//            if (index + 1 < numberOfIndices) { //Plus 1 because it is checked twice, so it will be incrimented twice. Checks to make sure no data outside of the array is accessed.
//                
//                //Sets the point location based on the location in the board and on the modifiers above.
//                vertices[index] = locationOfFirstPoint - (x * pointDistance);
//                index++;
//                
//                vertices[index] = locationOfFirstPoint - (y * pointDistance);
//                index++;
//            }
//        }
//    }
//    
//    for (int a = 0; a < this->numberOfTiles * 2; a++) { //2 for each tile to hold both an x and y coordinate
//        this->vertexData[a] = vertices[a];
//    }
//    
//    //Terrain and creature data. One for each tile
//    numberOfIndices = this->numberOfTiles;
//    
//    GLint terrains[numberOfIndices];
//    GLint creatures[numberOfIndices];
//    GLint directions[numberOfIndices];
//    GLint creatureControllers[numberOfIndices];
//    GLfloat colors[3 * numberOfIndices];
//    GLint buildings[numberOfIndices];
//    GLint buildingControllers[numberOfIndices];
//    
//    index = 0;
//    
//    for (GLuint x = 0; x < this->boardWidth; x++) {
//        for (GLuint y = 0; y < this->boardHeight; y++) {
//            if (index < numberOfIndices) { //Checks to make sure no data outside of the array is accessed.
//                if (setTerrainData)
//                    //Gets the terrain of the tile
//                    terrains[index] = this->game.board()->get(x, y).terrain();
//                
//                if (setCreatureData) {
//                    //Gets the creature on the tile
//                    creatures[index] = this->game.board()->get(x, y).creatureType();
//                    
//                    //Gets the direction if there is a creature there
//                    if (this->game.board()->get(x, y).creature() != nullptr) {
//                        directions[index] = this->game.board()->get(x, y).creature()->direction();
//                        creatureControllers[index] = this->game.board()->get(x, y).creature()->controller();
//                    } else {
//                        directions[index] = NORTH;
//                        creatureControllers[index] = 0;
//                    }
//                }
//                if (setColorData) {
//                    //Gets the color alteration of the tile
//                    colors[3 * index] = this->game.board()->get(x, y).color().x;
//                    colors[(3 * index) + 1] = this->game.board()->get(x, y).color().y;
//                    colors[(3 * index) + 2] = this->game.board()->get(x, y).color().z;
//                }
//                if (setBuildingData) {
//                    //Gets the building of the tile
//                    buildings[index] = this->game.board()->get(x, y).buildingType();
//                    
//                    //Gets the direction if there is a creature there
//                    if (this->game.board()->get(x, y).building() != nullptr) {
//                        buildingControllers[index] = this->game.board()->get(x, y).building()->controller();
//                    } else {
//                        buildingControllers[index] = 0;
//                    }
//                }
//                
//                //Increment
//                index++;
//            }
//        }
//    }
//    
//    for (int a = 0; a < this->numberOfTiles; a++) {
//        if (setTerrainData)
//            this->terrainData[a] = terrains[a];
//        if (setCreatureData) {
//            this->creatureData[3 * a] = creatures[a];
//            this->creatureData[(3 * a) + 1] = directions[a];
//            this->creatureData[(3 * a) + 2] = creatureControllers[a];
//        }
//        if (setColorData) {
//            this->colorData[3 * a] = colors[3 * a];
//            this->colorData[(3 * a) + 1] = colors[(3 * a) + 1];
//            this->colorData[(3 * a) + 2] = colors[(3 * a) + 2];
//        }
//        if (setDamageData) {
//            this->damageData[a] = 0;
//        }
//        if (setOffsetData) {
//            this->offsetData[a] = 0;
//        }
//        if (setBuildingData) {
//            this->buildingData[2 * a] = buildings[a];
//            this->buildingData[(2 * a) + 1] = buildingControllers[a];
//        }
//    }
//}
//
////Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
//void Visualizer::setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData, bool setBuildingData) {
//    //Distance between each seed point
//    GLfloat pointDistance = 0.2f;
//    
//    GLfloat locationOfFirstPoint = 0.0f;
//    locationOfFirstPoint += (this->game.board()->width() * pointDistance / 2.0f); //Sets the board halfway behind 0 and halfway in front
//    locationOfFirstPoint += (pointDistance / 2.0f); //Otherwise the 0.2 distance would be after each point (as if they were right-aligned). Instead they are center-aligned essentially.
//    
//    //Vertex data
//    GLuint numberOfIndices = this->numberOfTiles * INDICES_PER_TILES;
//    
//    GLuint index = 0;
//    
//    if (setVertexData) {
//        GLfloat vertices[numberOfIndices];
//        
//        for (GLuint x = 0; x < this->game.board()->width(); x++) {
//            for (GLuint y = 0; y < this->game.board()->height(x); y++) {
//                if (index + 1 < numberOfIndices) { //Plus 1 because it is checked twice, so it will be incrimented twice. Checks to make sure no data outside of the array is accessed.
//                    
//                    //Sets the point location based on the location in the board and on the modifiers above.
//                    vertices[index] = locationOfFirstPoint - (x * pointDistance);
//                    index++;
//                    
//                    vertices[index] = locationOfFirstPoint - (y * pointDistance);
//                    index++;
//                }
//            }
//        }
//        
//        for (int a = 0; a < this->numberOfTiles * INDICES_PER_TILES; a++) {
//            this->vertexData[a] = vertices[a];
//        }
//    }
//    
//    //Terrain and creature data. One for each tile
//    numberOfIndices = this->numberOfTiles;
//    
//    GLint terrains[numberOfIndices];
//    GLint creatures[numberOfIndices];
//    GLint directions[numberOfIndices];
//    GLint creatureControllers[numberOfIndices];
//    GLfloat colors[3 * numberOfIndices];
//    GLint buildings[numberOfIndices];
//    GLint buildingControllers[numberOfIndices];
//    
//    index = 0;
//    
//    for (GLuint x = 0; x < this->game.board()->width(); x++) {
//        for (GLuint y = 0; y < this->game.board()->height(x); y++) {
//            if (index < numberOfIndices) { //Checks to make sure no data outside of the array is accessed.
//                if (setTerrainData)
//                    //Gets the terrain of the tile
//                    terrains[index] = this->game.board()->get(x, y).terrain();
//                
//                if (setCreatureData) {
//                    //Gets the creature on the tile
//                    creatures[index] = this->game.board()->get(x, y).creatureType();
//                    
//                    //Gets the direction if there is a creature there
//                    if (this->game.board()->get(x, y).creature() != nullptr) {
//                        directions[index] = this->game.board()->get(x, y).creature()->direction();
//                        creatureControllers[index] = this->game.board()->get(x, y).creature()->controller();
//                    } else {
//                        directions[index] = NORTH;
//                        creatureControllers[index] = 0;
//                    }
//                }
//                if (setColorData) {
//                    //Gets the color alteration of the tile
//                    colors[3 * index] = this->game.board()->get(x, y).color().x;
//                    colors[(3 * index) + 1] = this->game.board()->get(x, y).color().y;
//                    colors[(3 * index) + 2] = this->game.board()->get(x, y).color().z;
//                }
//                if (setBuildingData) {
//                    //Gets the building of the tile
//                    buildings[index] = this->game.board()->get(x, y).buildingType();
//                    
//                    //Gets the direction if there is a creature there
//                    if (this->game.board()->get(x, y).building() != nullptr) {
//                        buildingControllers[index] = this->game.board()->get(x, y).building()->controller();
//                    } else {
//                        buildingControllers[index] = 0;
//                    }
//                }
//                
//                //Increment
//                index++;
//            }
//        }
//    }
//    
//    for (int a = 0; a < this->numberOfTiles; a++) {
//        if (setTerrainData)
//            this->terrainData[a] = terrains[a];
//        if (setCreatureData) {
//            this->creatureData[3 * a] = creatures[a];
//            this->creatureData[(3 * a) + 1] = directions[a];
//            this->creatureData[(3 * a) + 2] = creatureControllers[a];
//        }
//        if (setColorData) {
//            this->colorData[3 * a] = colors[3 * a];
//            this->colorData[(3 * a) + 1] = colors[(3 * a) + 1];
//            this->colorData[(3 * a) + 2] = colors[(3 * a) + 2];
//        }
//        if (setDamageData) {
//            this->damageData[a] = 0;
//        }
//        if (setOffsetData) {
//            this->offsetData[a] = 0;
//        }
//        if (setBuildingData) {
//            this->buildingData[2 * a] = buildings[a];
//            this->buildingData[(2 * a) + 1] = buildingControllers[a];
//        }
//    }
//}

////Initialize OpenGL buffers with the object's vertex data.
//void Visualizer::setBuffers(std::map<BoardInfoDataTypes, std::string> boardInfo) {
//
//    //The data arrays that hold ints are converted implicitly directly from chars
//    //The data arrays that hold floats are converted by dividing the char by 100. This means the float can have at most 2 decimal places, and must be between -1.28 and 1.27
//    for (GLuint tile = 0; tile < this->numberOfTiles; tile++) {
//        this->terrainData[tile] = boardInfo[TerrainData][tile]; //char -> int
//        this->creatureData[tile] = boardInfo[CreatureData][tile]; //char -> int
//        this->colorData[tile] = boardInfo[ColorData][tile] / 100; //char / 100 -> int
//        this->damageData[tile] = boardInfo[DamageData][tile]; //char -> int
//        this->offsetData[tile] = boardInfo[OffsetData][tile] / 100; //char / 100 -> int
//        this->buildingData[tile] = boardInfo[BuildingData][tile]; //char -> int
//    }
    


void Visualizer::setBuffers(std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec) {
    
    this->terrainData = terrainDataVec;
    this->creatureData = creatureDataVec;
    this->colorData = colorDataVec;
    this->damageData = damageDataVec;
    this->offsetData = offsetDataVec;
    this->buildingData = buildingDataVec;
    
    //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->vertexVBO);
    glGenBuffers(1, &this->terrainVBO);
    glGenBuffers(1, &this->creatureVBO);
    glGenBuffers(1, &this->colorVBO);
    glGenBuffers(1, &this->damageVBO);
    glGenBuffers(1, &this->offsetVBO);
    glGenBuffers(1, &this->buildingVBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Vertex VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData.data()), this->vertexData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Terrain VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->terrainData.data()), this->terrainData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Creature VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData.data()), this->creatureData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Color VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData.data()), this->colorData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    //Damage VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData.data()), this->damageData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    //Offset VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->offsetData.data()), this->offsetData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    //Building VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->buildingData.data()), this->buildingData.data(), GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(6);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

void Visualizer::setInterface() {
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(this->gameWindow, &viewportWidth, &viewportHeight);
    
    this->leftInterfaceStats = interfaceStat(0.0, 0.0, viewportWidth / 6.0, viewportHeight);
    this->bottomInterfaceStats = interfaceStat(viewportWidth * 1.0 / 6.0, 0.0, viewportWidth * 2.0 / 3.0, viewportHeight / 4.0);
    this->rightInterfaceStats = interfaceStat(viewportWidth * 5.0 / 6.0, 0.0, viewportWidth / 6.0, viewportHeight);
    this->settingsMenuStats = interfaceStat(viewportWidth/ 3.0, viewportHeight / 6.0, viewportWidth / 3.0, viewportHeight * 2.0 / 3.0);
    
    this->interfaceShader = Shader("Shaders/interface/interface.vert", "Shaders/interface/interface.frag");
    
    this->buttonShader = Shader("Shaders/button/button.vert", "Shaders/button/button.frag");
    
    this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
    //this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
    
    //Left-Side Game UI (brown rectangle)
    this->interfaces[default_left] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->leftInterfaceStats.x, this->leftInterfaceStats.y, this->leftInterfaceStats.width, this->leftInterfaceStats.height, default_left);
    
    //Bottom Game UI (brown rectangle)
    this->interfaces[default_bottom] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->bottomInterfaceStats.x, this->bottomInterfaceStats.y, this->bottomInterfaceStats.width, this->bottomInterfaceStats.height, default_bottom);
    
    //Right-Side Game UI (brown rectangle)
    this->interfaces[default_right] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, default_right);
    
    //Interface for selected creatures
    this->interfaces[creature] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, creature);
    
    //Interface for selected buildings
    this->interfaces[building] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, building);
    
    //Settings popup menu
    this->interfaces[settings] = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, this->gameWindow, this->settingsMenuStats.x, this->settingsMenuStats.y, this->settingsMenuStats.width, this->settingsMenuStats.height, settings);
    
    this->darkenBox = Box(this->buttonShader, this->gameWindow, 0, 0, this->windowWidth, this->windowHeight, 0, 0, this->windowWidth, this->windowHeight, glm::vec4(0.0, 0.0, 0.0, 0.5), "", other); //Set the box that will darken the screen while a settings menu is up
}

//Loads a texture into the back of the vector of texture objects. Only works up to 32 times. Throws an error if there are already 32 textures.
void Visualizer::loadTexture(const GLchar* texPath, const GLchar* texName) {
    if (textures.size() <= 32)
        textures.push_back(Texture(texPath, (GLuint)textures.size(), texName));
    else
        throw std::range_error("32 textures already loaded.");
}

//Replaces the designated spot in the vector of texture objects with a new texture. Throws an error if the desired index is out of vector range.
void Visualizer::replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName) {
    if (texIndex < textures.size())
        textures[texIndex] = Texture(texPath, texIndex, texName);
    else
        throw std::range_error("No texture loaded in that spot.");
}

//Contains matrix transformations to be done on the board. This sets model and projection matrices. Called only once
void Visualizer::presetTransformations() {
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
}

//A function to update all of the buffers that need to be updated. Should be called every frame.
//void Visualizer::updateBuffers(std::map<BoardInfoDataTypes, std::string> boardInfo) {
void Visualizer::updateBuffers(std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec) {
    
//    //The data arrays that hold ints are converted implicitly directly from chars
//    //The data arrays that hold floats are converted by dividing the char by 100. This means the float can have at most 2 decimal places, and must be between -1.28 and 1.27
//    for (GLuint tile = 0; tile < this->numberOfTiles; tile++) {
//        this->terrainData[tile] = boardInfo[TerrainData][tile]; //char -> int
//
//        this->creatureData[3 * tile] = boardInfo[CreatureData][3 * tile]; //char -> int
//        this->creatureData[(3 * tile) + 1] = boardInfo[CreatureData][(3 * tile) + 1]; //char -> int
//        this->creatureData[(3 * tile) + 2] = boardInfo[CreatureData][(3 * tile) + 2]; //char -> int
//
//        this->colorData[3 * tile] = boardInfo[ColorData][3 * tile] / 100; //char / 100 -> int
//        this->colorData[(3 * tile) + 1] = boardInfo[ColorData][(3 * tile) + 1] / 100; //char / 100 -> int
//        this->colorData[(3 * tile) + 2] = boardInfo[ColorData][(3 * tile) + 2] / 100; //char / 100 -> int
//
//        this->damageData[tile] = boardInfo[DamageData][tile]; //char -> int
//
//        this->offsetData[tile] = boardInfo[OffsetData][tile] / 100; //char / 100 -> int
//
//        this->buildingData[2 * tile] = boardInfo[BuildingData][2 * tile]; //char -> int
//        this->buildingData[(2 * tile) + 1] = boardInfo[BuildingData][(2 * tile) + 1]; //char -> int
//    }
    
    this->terrainData = terrainDataVec;
    
    this->creatureData = creatureDataVec;
    
    this->colorData = colorDataVec;
    
    this->damageData = damageDataVec;
    
    this->offsetData = offsetDataVec;
    
    this->buildingData = buildingDataVec;
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->terrainData.data()), this->terrainData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData.data()), this->creatureData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData.data()), this->colorData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData.data()), this->damageData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->offsetData.data()), this->offsetData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->buildingData.data()), this->buildingData.data(), GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(6);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

    /*
    
    
    
    //Set the offset VBO
    
    //Update creature data array and buildings
    this->setData(false, false, true, false, false, false, true);
    
    //Goes through all tiles and continues moving any that are moving
    for (GLuint tile = 0; tile < this->numberOfTiles; tile++) {
        
        glm::ivec2 boardLoc;
        boardLoc.x = tile / this->game.board()->width();
        boardLoc.y = tile - (this->game.board()->width() * boardLoc.x);
        
        Creature* creature = this->game.board()->get(boardLoc.x, boardLoc.y).creature();
        
        if (creature != nullptr) {
            this->offsetData[tile] = creature->offset();
        } else {
            this->offsetData[tile] = 0.0;
        }
    }
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->offsetData), this->offsetData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Now set the creature VBO
    
    //Update creature data array. This is done again so that if the offset was adjusted and the creature moved, that is reflected in the creature VBO and rendered. Otherwise, the creature would momentarily blink and flash back to its previous location
    this->setData(false, false, true, false, false, false, false);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the creature VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Bind the building VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->buildingData), this->buildingData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(6);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Update creature data array
    this->setData(false, false, true, false, false, false, true);
    
    //Update creature data array
    this->setData(false, false, false, true, false, false, true);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData), this->colorData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Goes through existence times and updates them based on glfwGetTime()
    for (GLuint tile = 0; tile < this->numberOfTiles; tile++) {
        glm::ivec2 boardLoc;
        boardLoc.x = tile / this->game.board()->width();
        boardLoc.y = tile - (this->game.board()->width() * boardLoc.x);
        
        Tile currentTile = this->game.board()->get(boardLoc.x, boardLoc.y);
        
        if (glfwGetTime() - currentTile.timeOfDamage() > Tile::damageBoxTime) { //Don't show the damage if it is not new
            this->damageData[tile] = 0;
        } else {
            this->damageData[tile] = currentTile.damage();
        }
    }
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData), this->damageData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}*/

void Visualizer::updateInterfaces() {
    this->leftInterface = &this->interfaces[default_left];
    this->bottomInterface = &this->interfaces[default_bottom];
    this->rightInterface = &this->interfaces[default_right];
    
    //If the selected tile is on the board
    if (this->selectedTile.x >= 0 && this->selectedTile.x < this->boardWidth && selectedTile.y >= 0 && this->selectedTile.y < this->boardHeight) {
        
//        Tile tile = this->game.board()->get(selectedTile.x, selectedTile.y);
//        
//        if (tile.creature() != nullptr) {
//            //Set the right interface to be the creature if there is a creature at the selected tile
//            this->rightInterface = &this->interfaces[creature];
//            
//            //Update the boxes to display creature stats
//            if (this->interfaces[creature].boxes.size() > 0) {
//                this->interfaces[creature].boxes[creature_attack].text = "Attack: " + std::to_string(tile.creature()->attack());
//                this->interfaces[creature].boxes[creature_range].text = "Range: " + std::to_string(tile.creature()->range());
//                this->interfaces[creature].boxes[creature_vision].text = "Vision: " + std::to_string(tile.creature()->vision());
//                this->interfaces[creature].boxes[creature_race].text = tile.creature()->raceString();
//            }
//            
//            //Update the display bars to display the creature quantities, like health and energy, which change
//            if (this->interfaces[creature].displayBars.size() > 0) {
//                
//                this->interfaces[creature].displayBars[HealthBar].setValue(tile.creature()->health());
//                this->interfaces[creature].displayBars[HealthBar].setMaxValue(tile.creature()->maxHealth());
//                this->interfaces[creature].displayBars[HealthBar].text = "Health: " + std::to_string((int)tile.creature()->health()) + "/" + std::to_string((int)tile.creature()->maxHealth());
//                
//                this->interfaces[creature].displayBars[EnergyBar].setValue(tile.creature()->energy());
//                this->interfaces[creature].displayBars[EnergyBar].setMaxValue(tile.creature()->maxEnergy());
//                this->interfaces[creature].displayBars[EnergyBar].text = "Energy: " + std::to_string((int)tile.creature()->energy()) + "/" + std::to_string((int)tile.creature()->maxEnergy());
//                
//                
//            }
//        }
//        
//        if (tile.building() != nullptr) {
//            //Do the same for buildings
//            this->rightInterface = &this->interfaces[building];
//            
//            if (this->interfaces[building].displayBars.size() > 0) {
//                this->interfaces[building].displayBars[HealthBar].setValue(tile.building()->health());
//                this->interfaces[building].displayBars[HealthBar].setMaxValue(tile.building()->maxHealth());
//                this->interfaces[building].displayBars[HealthBar].text = "Health: " + std::to_string((int)tile.building()->health()) + "/" + std::to_string((int)tile.building()->maxHealth());
//            }
//            
//            if (this->interfaces[building].buttons.size() > 0) {
//                this->interfaces[building].buttons[0].text = tile.building()->buttonText();
//                this->interfaces[building].buttons[0].action = tile.building()->action();
//            }
//        }
    }
}

void Visualizer::renderDamageText() {
    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
    }
    
    //Goes through existence times and updates them based on glfwGetTime()
    for (GLuint tile = 0; tile < this->numberOfTiles ; tile++) {
        
        if (this->damageData[tile] != 0) { //Don't show the damage if it is not new
        
            glm::ivec2 windowSize;
            glm::ivec2 framebufferSize;
            glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
            glfwGetFramebufferSize(this->gameWindow, &framebufferSize.x, &framebufferSize.y);

            glm::vec2 damageTileCoords = tileCenters[tile];
            
            damageTileCoords.x += 1.0;
            damageTileCoords.x /= 2.0;
            damageTileCoords.y += 1.0;
            damageTileCoords.y /= 2.0;
            
            glm::vec2 fontSize = this->font.getSize(std::to_string(this->damageData[tile]), 1.0);
            
            this->font.render(std::to_string(this->damageData[tile]), damageTileCoords.x * viewportSize.x - (fontSize.x / 2), damageTileCoords.y * this->viewportSize.y - (fontSize.y / 2), 1.0, glm::vec3(1.0, 1.0, 1.0), this->viewportSize.x, this->viewportSize.y);
        }
    }
}

//A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
void Visualizer::moveCamera() {
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

void Visualizer::processButton(std::string action) {
//    if (action == "settings") {
//        
//        this->showSettings = true;
//        
//    } else if (action == "next turn") { //Process the button indicating to move to the next turn
//        
//        this->game.nextTurn();
//        
//    } else if (action.find("creature,") != std::string::npos) { //Basically if the string action contains "creature", the button makes a creature
//        if (this->game.tileSelected() != NO_SELECTION && this->game.tileSelected() != INTERFACE_BOX_SELECTION && !this->game.board()->get(this->game.tileSelected().x, this->game.tileSelected().y).occupied()) {
//            
//            glm::ivec2 selectedTile = this->game.tileSelected();
//            
//            //Interpret the string to find out what kind of creature
//            
//            /* The contents of the button string are:
//             * creature,[race],[maxHealth],[maxEnergy],[attack],[vision],[range],[cost],[start direction]
//             *
//             * Each value in brackets indicates a number or enum that represents that value. Each of these values are separated by commas.
//             *
//             * This function goes through the string and extracts those values and constructs a creature based on them.
//             */
//            
//            Race race = Human;
//            AttackStyle attackStyle = LightMelee;
//            GLuint values[] = {0, 0, 0, 0, 0, 0};
//            GLuint direction;
//            
//            action.erase(0, 9); //Gets rid of the "creature," from the string
//            
//            //Extract the race of the creature
//            
//            if (action.compare(0, 5, "Human") == 0) {
//                race = Human;
//                action.erase(0, 6); //Gets rid of "Human,". This is 1 more than the number of characters in "Human" to also get rid of the comma. Same is true with the other races.
//            } else if (action.compare(0, 3, "Elf") == 0) {
//                race = Elf;
//                action.erase(0, 4);
//            } else if (action.compare(0, 5, "Dwarf") == 0) {
//                race = Dwarf;
//                action.erase(0, 6);
//            } else if (action.compare(0, 3, "Orc") == 0) {
//                race = Orc;
//                action.erase(0, 4);
//            } else if (action.compare(0, 6, "Goblin") == 0) {
//                race = Goblin;
//                action.erase(0, 7);
//            } else if (action.compare(0, 6, "Undead") == 0) {
//                race = Undead;
//                action.erase(0, 7);
//            } else if (action.compare(0, 7, "Vampire") == 0) {
//                race = Vampire;
//                action.erase(0, 8);
//            }
//            
//            //Extract the numerical values of the creature
//            
//            for (GLuint valueNum = 0; valueNum < 6; valueNum++) {
//                //Find the position of the next comma, which is the number of digits before that comma
//                GLuint numDigits = (GLuint)action.find(',');
//                
//                for (GLint place = numDigits - 1; place >= 0; place--) {
//                    values[valueNum] += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
//                    action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
//                }
//                
//                action.erase(0, 1); //Get rid of the comma
//            }
//            
//            if (action.compare(0, 5, "NORTH") == 0) {
//                direction = NORTH;
//                action.erase(0, 6); //Gets rid of "NORTH,". This is 1 more than the number of characters in "NORTH" to also get rid of the comma. Same is true with the other directions.
//            } else if (action.compare(0, 4, "EAST") == 0) {
//                direction = EAST;
//                action.erase(0, 5);
//            } else if (action.compare(0, 5, "SOUTH") == 0) {
//                direction = SOUTH;
//                action.erase(0, 6);
//            } else if (action.compare(0, 4, "WEST") == 0) {
//                direction = WEST;
//                action.erase(0, 5);
//            }
//            
//            Creature newCreature(selectedTile.x, selectedTile.y, race, values[0], values[1], values[2], attackStyle, values[3], values[4], values[5], direction, this->game.activePlayer());
//            
//            if (this->game.board()->get(selectedTile.x, selectedTile.y).passableByCreature(newCreature)) {
//                try {
//                    this->game.board()->setCreature(selectedTile.x, selectedTile.y, newCreature);
//                    
//                    //Reset all tiles to be unselected now that the creature has been added
//                    for (GLuint x = 0; x < this->game.board()->width(); x++) {
//                        for (GLuint y = 0; y < this->game.board()->height(x); y++) {
//                            this->game.board()->setStyle(x, y, Regular);
//                        }
//                    }
//                    
//                    this->game.selectTile(NO_SELECTION.x, NO_SELECTION.y);
//                    
//                } catch (std::exception) {
//                    //For now, nothing needs to be done if there isn't a selected tile that wasn't caught above. Later, if a banner of error or something is shown, that can be added here too
//                    std::cout << "Error adding creature" << std::endl;
//                }
//            }
//        }
//    } else if (action.find("building_new_creature") != std::string::npos) { //Basically if the string action contains "building", the button follows the building instructions
//        
//        //For now, set adjacent spots as reachable and create a creature on the selected one
//        
//        action.erase(0, 22); //Delete "building_new_creature(" from the action string
//        
//        glm::ivec2 buildingPos = glm::ivec2(0, 0);
//        
//        //Extract the building position
//        
//        GLuint numDigits = (GLuint)action.find(',');
//        
//        for (GLint place = numDigits - 1; place >= 0; place--) {
//            buildingPos.x += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
//            action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
//        }
//        
//        action.erase(0, 1); //Get rid of the comma
//        
//        numDigits = (GLuint)action.find(')');
//        
//        for (GLint place = numDigits - 1; place >= 0; place--) {
//            buildingPos.y += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
//            action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
//        }
//        
//        action.erase(0, 1); //Get rid of the parenthasis
//        
//        //If the position is within the board
//        if (buildingPos.x >= 0 && buildingPos.x < this->game.board()->width() && buildingPos.y >= 0 && buildingPos.y < this->game.board()->height(buildingPos.x)) {
//            
//            if (this->game.board()->get(buildingPos.x, buildingPos.y).building() != nullptr && this->game.activePlayer() == this->game.board()->get(buildingPos.x, buildingPos.y).building()->controller()) {
//            
//                //North
//                if (buildingPos.y > 0) {
//                    this->game.board()->setStyle(buildingPos.x, buildingPos.y - 1, Reachable);
//                }
//                
//                //East
//                if (buildingPos.x > 0) {
//                    this->game.board()->setStyle(buildingPos.x - 1, buildingPos.y, Reachable);
//                }
//                
//                //South
//                if (buildingPos.y < this->game.board()->height(buildingPos.x) - 1) {
//                    this->game.board()->setStyle(buildingPos.x, buildingPos.y + 1, Reachable);
//                }
//                
//                //West
//                if (buildingPos.y < this->game.board()->width() - 1) {
//                    this->game.board()->setStyle(buildingPos.x + 1, buildingPos.y, Reachable);
//                }
//            }
//        }
//    } else if (action.find("building,") != std::string::npos) { //Basically if the string action contains "creature", the button makes a creature
//        if (this->game.tileSelected() != NO_SELECTION && this->game.tileSelected() != INTERFACE_BOX_SELECTION && !this->game.board()->get(this->game.tileSelected().x, this->game.tileSelected().y).occupied()) {
//            
//            glm::ivec2 selectedTile = this->game.tileSelected();
//            
//            //Interpret the string to find out what kind of building
//            
//            /* The contents of the button string are:
//             * building,[maxHealth],[cost]
//             *
//             * Each value in brackets indicates a number or enum that represents that value. Each of these values are separated by commas.
//             *
//             * This function goes through the string and extracts those values and constructs a building based on them.
//             */
//            GLuint values[] = {0, 0};
//            
//            action.erase(0, 9); //Gets rid of the "building," from the string
//            
//            //Extract the numerical values of the building
//            
//            for (GLuint valueNum = 0; valueNum < 2; valueNum++) {
//                //Find the position of the next comma, which is the number of digits before that comma
//                GLuint numDigits = (GLuint)action.find(',');
//                
//                for (GLint place = numDigits - 1; place >= 0; place--) {
//                    values[valueNum] += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
//                    action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
//                }
//                
//                action.erase(0, 1); //Get rid of the comma
//            }
//            
//            Building newBuilding(selectedTile.x, selectedTile.y, "Make creature", "building_new_creature(" + std::to_string(selectedTile.x) + "," + std::to_string(selectedTile.y) + ")", values[0], values[1], this->game.activePlayer());
//            
//            if (!this->game.board()->get(selectedTile.x, selectedTile.y).occupied()) {
//                try {
//                    this->game.board()->setBuilding(selectedTile.x, selectedTile.y, newBuilding);
//                    
//                    //Reset all tiles to be unselected now that the creature has been added
//                    for (GLuint x = 0; x < this->game.board()->width(); x++) {
//                        for (GLuint y = 0; y < this->game.board()->height(x); y++) {
//                            this->game.board()->setStyle(x, y, Regular);
//                        }
//                    }
//                    
//                    this->game.selectTile(NO_SELECTION.x, NO_SELECTION.y);
//                    
//                } catch (std::exception) {
//                    //For now, nothing needs to be done if there isn't a selected tile that wasn't caught above. Later, if a banner of error or something is shown, that can be added here too
//                    std::cout << "Error adding building" << std::endl;
//                }
//            }
//        }
//    }
}

void Visualizer::renderSettingsMenu(bool mouseUp, bool mouseDown) {
    glViewport(0, 0, this->leftInterfaceStats.width + this->bottomInterfaceStats.width + this->rightInterfaceStats.width, this->leftInterfaceStats.height);
    glScissor(0, 0, this->leftInterfaceStats.width + this->bottomInterfaceStats.width + this->rightInterfaceStats.width, this->leftInterfaceStats.height);
    
    this->darkenBox.render();
    
    this->interfaces[settings].render(mouseUp, mouseDown, true);
    
}

glm::ivec2 Visualizer::mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, std::vector<glm::vec4> tileCenters) {
    GLint tileIndex = -1; //The tile index where the mouse was clicked. Initialized as -1 to mean no index found
    
    //If x is in the last sixth or the first sixth, ignore the click because the interface boxes were clicked
    if (mousePos.x > (windowSize.x * 5.0 / 6.0) || mousePos.x < (windowSize.x / 6.0))
        return INTERFACE_BOX_SELECTION;
    
    //Only the middle 2/3 of the screen is the board, so make the start of that section 0
    mousePos.x -= (windowSize.x / 6.0);
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    mousePos.x *= (3.0 / 2.0);
    
    //Do the same for y, except that only the bottom 1/4 of the screen is not part of the board
    if (mousePos.y > (windowSize.y * 3.0 / 4.0))
        return INTERFACE_BOX_SELECTION;
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    mousePos.y *= (4.0 / 3.0);
    
    //Make mousePos between 0 and 1 by dividing the position by the maximum position (width or height)
    mousePos.x /= windowSize.x;
    mousePos.y /= windowSize.y;
    
    //Now make it 0 to 2 by doubling it
    mousePos.x *= 2.0f;
    mousePos.y *= 2.0f;
    
    //Now subtract 1 to get it between -1 and 1
    mousePos.x -= 1.0f;
    mousePos.y -= 1.0f;
    
    //So that -1 is the bottom of the screen, not the top
    mousePos.y = -mousePos.y;
    
    /*for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
     //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
     tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
     }*/
    
    //The distance from one point to the horizontal point and the vertical point:
    
    //The points diagonally above and below each vertex become horizontal and vertical after rotation. To find them, find the point below the vertex and add one and subtract one.
    
    if (this->boardWidth * this->boardWidth < this->boardWidth + 1) { //In case finding the distances (just below) would cause a bad access
        throw std::length_error("Board too small; Board size: (" + std::to_string(this->boardWidth) + ", " + std::to_string(this->boardHeight) + ")");
    }
    
    GLfloat distance1 = Visualizer::getDistance(tileCenters[0], tileCenters[0 + this->boardWidth + 1]); //Diagonal down and to the right
    GLfloat distance2 = Visualizer::getDistance(tileCenters[1], tileCenters[1 + this->boardWidth - 1]); //Diagonal down and to the left
    
    //Distance horizontally is double the distance of the vertical one because it was compressed vertically.
    //The horizontal distance is the max of the above distances, and the vertical distance the minimum
    
    GLfloat verticalDistance = fminf(distance1, distance2);
    GLfloat horizontalDistance = fmaxf(distance1, distance2);
    
    //For every point, check if it is within the boundaries of the respective diamond's bounds, by finding the 4 bounding lines of that rectange
    
    GLfloat slope = verticalDistance / horizontalDistance; // = rise / run
    
    //Using line equation:
    // y = slope ( x - h ) + k
    //Where (h,k) is a point on the line
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        glm::vec2 center = glm::vec2(tileCenters[index].x, tileCenters[index].y);
        
        bool pointInIndex = true;
        
        //Lower left inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y > ( -slope ) ( x - h ) + k
        // (h,k) is the point below the center
        
        GLfloat h = center.x;
        GLfloat k = center.y - (verticalDistance / 2.0);
        
        if (mousePos.y < ( -slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
            continue;
        }
        
        //Lower right inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y > ( slope ) ( x - h ) + k
        // (h,k) is the point below the center, the same as previously
        
        if (mousePos.y < ( slope ) * ( mousePos.x - h ) + k) { //If it's below this line
            pointInIndex = false;
            continue;
        }
        
        //Upper left inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y < ( slope ) ( x - h ) + k
        // (h,k) is the point above the center
        
        h = center.x; //h stays the same
        k = center.y + (verticalDistance / 2.0);
        
        if (mousePos.y > ( slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
            continue;
        }
        //Upper right inequality: (if this does NOT hold then the point isn't in the region. We check if this is false)
        // y < ( -slope ) ( x - h ) + k
        // (h,k) is the point above the center, the same as previously
        
        if (mousePos.y > ( -slope ) * ( mousePos.x - h ) + k) { //If it's above this line
            pointInIndex = false;
            continue;
        }
        
        if (pointInIndex) { //The point was in bounds
            tileIndex = index;
            break; //Point found, no need to search more
        }
    }
    
    //If no tile was found, -1 is returned. Otherwise, the index pointing to the coordinate in the array of glm::vec2's is returned
    //Since there are double the number of coordinates, this coordinate times 2 is the first coordinate of the tile in vertexData
    
    //Return negative coordinates if the click is outside of all tiles
    if (tileIndex == -1)
        return NO_SELECTION;
    
    glm::ivec2 tileIndexVec;
    
    tileIndexVec.x = (int)(tileIndex / this->boardWidth); //The x index in the 2D vector
    
    tileIndexVec.y = tileIndex - (this->boardWidth * tileIndexVec.x); //The y index in the 2D vector
    
    return tileIndexVec;
}

//A function GLFW can call when a key event occurs
void Visualizer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS && mods == GLFW_MOD_SUPER) { //Command-W: close the application
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { //Escape: open settings menu
        escClicked = true;
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
void Visualizer::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseDown = true;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        mouseUp = true;
    else
        mouseUp = false;
}
