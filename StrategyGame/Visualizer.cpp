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

//Constructor without geometry shader
Visualizer::Visualizer(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : game(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true, true, true, true); //Set all of the data arrays with information from the board
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    this->gameShader = Shader(vertexPath, fragmentPath);
    
    this->font = Font(FONT_PATH);
    
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
    try {
        this->loadTexture("Resources/tower.png", "towerTex");
    } catch (std::exception e) {
        std::cout << "Error loading numbers texture: " << e.what();
    }
    try {
        this->loadTexture("Resources/circle.png", "circleTex");
    } catch (std::exception e) {
        std::cout << "Error loading circle texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Constructor with geometry shader
Visualizer::Visualizer(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : game(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true, true, true, true); //Set the data arrays with information from the board
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    glm::ivec2 windowSize;
    glm::ivec2 framebufferSize;
    
    glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
    glfwGetFramebufferSize(this->gameWindow, &framebufferSize.x, &framebufferSize.y);
    
    this->gameShader = Shader(vertexPath, geometryPath, fragmentPath);
    
    this->font = Font(FONT_PATH);
    
    this->setInterface();
    
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
    try {
        this->loadTexture("Resources/tower.png", "towerTex");
    } catch (std::exception e) {
        std::cout << "Error loading numbers texture: " << e.what();
    }
    try {
        this->loadTexture("Resources/circle.png", "circleTex");
    } catch (std::exception e) {
        std::cout << "Error loading circle texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Public member functions

//A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
void Visualizer::render() {
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
    
    //If the mouse was clicked, set the color of the tile that was clicked
    if (mouseDown) {
        //Set mouseDown to false because this next function deals with the mouse and updates accordingly.
        mouseDown = false;
        
        glm::dvec2 cursorPos;
        glm::ivec2 windowSize;
        
        glfwGetCursorPos(this->gameWindow, &cursorPos.x, &cursorPos.y);
        glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
        
        glm::vec4 tileCenters[NUMBER_OF_TILES]; //Representing the center point of all of the map squares
        
        for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
            //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
            tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
        }
        
        //This function deals with mouse clicks. If the mouse was clicked in an interface box, mouseDown is returned to true so that the buttons can check if there is any click
        this->game.updateSelected(&mouseDown, cursorPos, windowSize, tileCenters);
    }
    
    this->game.updateCreatures(this->deltaTime);
    
    //Update the buffers that need updating.
    this->updateBuffers();
    
    //Set the camera-translation vector based on arrowkey inputs
    this->moveCamera();
    
    //Reset the view matrix
    this->view = glm::mat4();
    
    //Affect the camera position and send the view matrix to the shader
    this->view = glm::translate(this->view, cameraCenter);
    this->gameShader.uniformMat4("view", this->view);
    
    //Bind the VAO and draw shapes
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, NUMBER_OF_TILES);
    glBindVertexArray(0);
    
    //This doesn't work yet
//    this->renderDamageText();
    
    //Go through the interfaces and render them
    for (GLuint a = 0; a < interfaces.size(); a++) {
        this->interfaces[a].render(mouseDown, mouseUp); //This renders the interface and its buttons
        
        //Go through the buttons and check if they are pressed, and do any consequential actions
        for (auto button = this->interfaces[a].buttons.begin(); button != interfaces[a].buttons.end(); button++) {
            if (button->isPressed()) {
                this->processButton(button->action());
            }
        }
    }
    
    //mouseDown is likely set to false above, but not if the mouse was clicked in an interface box. In that case, the above for loop deals with it, and now it is no longer needed to be true, so it is reset
    if (mouseDown)
        mouseDown = false;
    
    //Swap buffers so as to properly render without flickering
    glfwSwapBuffers(this->gameWindow);
}

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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Non-resizable
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
    glViewport(viewportWidth / 6.0, viewportHeight / 4.0, viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfacecs
    
    this->viewportSize = glm::ivec2(viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0);
    
    //Set key callback function
    glfwSetKeyCallback(this->gameWindow, this->keyCallback);
    
    //Set mouse button click callback function
    glfwSetMouseButtonCallback(this->gameWindow, this->mouseButtonCallback);
}

//Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
void Visualizer::setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData, bool setBuildingData) {
    //Distance between each seed point
    GLfloat pointDistance = 0.2f;
    
    GLfloat locationOfFirstPoint = 0.0f;
    locationOfFirstPoint += (this->game.board()->width() * pointDistance / 2.0f); //Sets the board halfway behind 0 and halfway in front
    locationOfFirstPoint += (pointDistance / 2.0f); //Otherwise the 0.2 distance would be after each point (as if they were right-aligned). Instead they are center-aligned essentially.
    
    //Vertex data
    GLuint numberOfIndices = NUMBER_OF_TILES * INDICES_PER_TILES;
    
    GLuint index = 0;
    
    if (setVertexData) {
        GLfloat vertices[numberOfIndices];
        
        for (GLuint x = 0; x < this->game.board()->width(); x++) {
            for (GLuint y = 0; y < this->game.board()->height(x); y++) {
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
    GLint directions[numberOfIndices];
    GLint controllers[numberOfIndices];
    GLfloat colors[3 * numberOfIndices];
    GLint buildings[numberOfIndices];
    
    index = 0;
    
    for (GLuint x = 0; x < this->game.board()->width(); x++) {
        for (GLuint y = 0; y < this->game.board()->height(x); y++) {
            if (index < numberOfIndices) { //Checks to make sure no data outside of the array is accessed.
                if (setTerrainData)
                    //Gets the terrain of the tile
                    terrains[index] = this->game.board()->get(x, y).terrain();
                
                if (setCreatureData) {
                    //Gets the creature on the tile
                    creatures[index] = this->game.board()->get(x, y).creatureType();
                    
                    //Gets the direction if there is a creature there
                    if (this->game.board()->get(x, y).creature() != nullptr) {
                        directions[index] = this->game.board()->get(x, y).creature()->direction();
                        controllers[index] = this->game.board()->get(x, y).creature()->controller();
                    } else {
                        directions[index] = NORTH;
                        controllers[index] = 0;
                    }
                }
                if (setColorData) {
                    //Gets the color alteration of the tile
                    colors[3 * index] = this->game.board()->get(x, y).color().x;
                    colors[(3 * index) + 1] = this->game.board()->get(x, y).color().y;
                    colors[(3 * index) + 2] = this->game.board()->get(x, y).color().z;
                }
                if (setBuildingData) {
                    //Gets the building of the tile
                    buildings[index] = this->game.board()->get(x, y).buildingType();
                }
                
                //Increment
                index++;
            }
        }
    }
    
    for (int a = 0; a < NUMBER_OF_TILES; a++) {
        if (setTerrainData)
            this->terrainData[a] = terrains[a];
        if (setCreatureData) {
            this->creatureData[3 * a] = creatures[a];
            this->creatureData[(3 * a) + 1] = directions[a];
            this->creatureData[(3 * a) + 2] = controllers[a];
        }
        if (setColorData) {
            this->colorData[3 * a] = colors[3 * a];
            this->colorData[(3 * a) + 1] = colors[(3 * a) + 1];
            this->colorData[(3 * a) + 2] = colors[(3 * a) + 2];
        }
        if (setDamageData) {
            this->damageData[a] = 0;
        }
        if (setOffsetData) {
            this->offsetData[a] = 0;
        }
        if (setBuildingData) {
            this->buildingData[a] = buildings[a];
        }
    }
}

//Initialize OpenGL buffers with the object's vertex data.
void Visualizer::setBuffers() {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData), this->vertexData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, INDICES_PER_TILES * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Terrain VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->terrainData), this->terrainData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Creature VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Color VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData), this->colorData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    //Damage VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData), this->damageData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    //Offset VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->offsetData), this->offsetData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    //Building VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->buildingData), this->buildingData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(6);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

void Visualizer::setInterface() {
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(this->gameWindow, &viewportWidth, &viewportHeight);
    
    this->interfaceShader = Shader("Shaders/interface.vert", "Shaders/interface.frag");
    
    this->buttonShader = Shader("Shaders/button.vert", "Shaders/button.frag");
    
    //Left-Side Game UI (brown rectangle)
    this->interfaces.push_back(Interface(&this->interfaceShader, &this->buttonShader, this->gameWindow, 0.0, 0.0, viewportWidth / 6.0, viewportHeight, true));
    
    //Bottom Game UI (brown rectangle)
    this->interfaces.push_back(Interface(&this->interfaceShader, &this->buttonShader, this->gameWindow, viewportWidth * 1.0 / 6.0, 0.0, viewportWidth * 2.0 / 3.0, viewportHeight / 4.0, false));
    
    //Right-Side Game UI (brown rectangle)
    this->interfaces.push_back(Interface(&this->interfaceShader, &this->buttonShader, this->gameWindow, viewportWidth * 5.0 / 6.0, 0.0, viewportWidth / 6.0, viewportHeight, false));
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
void Visualizer::updateBuffers() {
    //Set the offset VBO
    
    //Update creature data array and buildings
    this->setData(false, false, true, false, false, false, true);
    
    //Goes through all tiles and continues moving any that are moving
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
        
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
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
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
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
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
}

//DOES NOT WORK AT THE MOMENT

/*void Visualizer::renderDamageText() {
    glm::vec4 tileCenters[NUMBER_OF_TILES]; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters[index] = this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f);
    }
    
    //Goes through existence times and updates them based on glfwGetTime()
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
        
        if (this->damageData[tile] != 0) { //Don't show the damage if it is not new
            glm::ivec2 windowSize;
            glm::ivec2 framebufferSize;
            glfwGetWindowSize(this->gameWindow, &windowSize.x, &windowSize.y);
            glfwGetFramebufferSize(this->gameWindow, &framebufferSize.x, &framebufferSize.y);
            
            glm::ivec2 damageTile;
            
            damageTile.x = tile / this->game.board()->width();
            damageTile.y = tile - damageTile.x;
            
            if (damageTile.x >= 0 && damageTile.x < this->game.board()->width()) {
                if (damageTile.y >= 0 && damageTile.y < this->game.board()->height(damageTile.x)) {
                    
                    glm::vec2 damageTileCoords = glm::vec2(tileCenters[damageTile.x * this->game.board()->width() + damageTile.y]);
                    
                    damageTileCoords.x += 1;
                    damageTileCoords.x /= 2;
                    
                    damageTileCoords.y += 1;
                    damageTileCoords.y /= 2;
                    
                    std::cout << "(" << damageTileCoords.x << ", " << damageTileCoords.y << ")" << std::endl;
                    
                    //Render the damage box
                    this->font.render(std::to_string(this->damageData[tile]), damageTileCoords.x, damageTileCoords.y, 1.0, glm::vec3(1.0, 1.0, 1.0), this->viewportSize.x, this->viewportSize.y);
                    
                    //Now use the shader so that the proper shader is used to render the game
                    this->gameShader.use();
                }
            }
        }
    }
}*/

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
    //Process the button indicating to move to the next turn
    if (action == "next turn") {
        
        this->game.nextTurn();
        
    } else if (action.find("creature") != std::string::npos) { //Basically if the string action contains "creature", the button makes a creature
        if (this->game.tileSelected() != NO_SELECTION && this->game.tileSelected() != INTERFACE_BOX_SELECTION && !this->game.board()->get(this->game.tileSelected().x, this->game.tileSelected().y).occupied()) {
            
            glm::ivec2 selectedTile = this->game.tileSelected();
            
            //Interpret the string to find out what kind of creature
            
            /* The contents of the button string are:
             * creature,[race],[maxHealth],[maxEnergy],[attack],[vision],[range],[cost],[start direction]
             *
             * Each value in brackets indicates a number or enum that represents that value. Each of these values are separated by commas.
             *
             * This function goes through the string and extracts those values and constructs a creature based on them.
             */
            
            Race race = Human;
            GLuint values[6] = {0, 0, 0, 0, 0, 0};
            GLuint direction;
            
            action.erase(0, 9); //Gets rid of the "creature," from the string
            
            //Extract the race of the creature
            
            if (action.compare(0, 5, "Human") == 0) {
                race = Human;
                action.erase(0, 6); //Gets rid of "Human,". This is 1 more than the number of characters in "Human" to also get rid of the comma. Same is true with the other races.
            } else if (action.compare(0, 3, "Elf") == 0) {
                race = Elf;
                action.erase(0, 4);
            } else if (action.compare(0, 5, "Dwarf") == 0) {
                race = Dwarf;
                action.erase(0, 6);
            } else if (action.compare(0, 3, "Orc") == 0) {
                race = Orc;
                action.erase(0, 4);
            } else if (action.compare(0, 6, "Goblin") == 0) {
                race = Goblin;
                action.erase(0, 7);
            } else if (action.compare(0, 6, "Undead") == 0) {
                race = Undead;
                action.erase(0, 7);
            } else if (action.compare(0, 7, "Vampire") == 0) {
                race = Vampire;
                action.erase(0, 8);
            }
            
            //Extract the numerical values of the creature
            
            for (GLuint valueNum = 0; valueNum < 6; valueNum++) {
                //Find the position of the next comma, which is the number of digits before that comma
                GLuint numDigits = (GLuint)action.find(',');
                
                for (GLint place = numDigits - 1; place >= 0; place--) {
                    values[valueNum] += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
                    action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
                }
                
                action.erase(0, 1); //Get rid of the comma
            }
            
            if (action.compare(0, 5, "NORTH") == 0) {
                direction = NORTH;
                action.erase(0, 6); //Gets rid of "NORTH,". This is 1 more than the number of characters in "NORTH" to also get rid of the comma. Same is true with the other directions.
            } else if (action.compare(0, 4, "EAST") == 0) {
                direction = EAST;
                action.erase(0, 5);
            } else if (action.compare(0, 5, "SOUTH") == 0) {
                direction = SOUTH;
                action.erase(0, 6);
            } else if (action.compare(0, 4, "WEST") == 0) {
                direction = WEST;
                action.erase(0, 5);
            }
            
            Creature newCreature(selectedTile.x, selectedTile.y, race, values[0], values[1], values[2], values[3], values[4], values[5], direction, this->game.activePlayer());
            
            if (this->game.board()->get(selectedTile.x, selectedTile.y).passableByCreature(newCreature)) {
                try {
                    this->game.board()->setCreature(selectedTile.x, selectedTile.y, newCreature);
                    
                    //Reset all tiles to be unselected now that the creature has been added
                    for (GLuint x = 0; x < this->game.board()->width(); x++) {
                        for (GLuint y = 0; y < this->game.board()->height(x); y++) {
                            this->game.board()->setStyle(x, y, Regular);
                        }
                    }
                    
                    selectedTile = NO_SELECTION;
                    
                } catch (std::exception) {
                    //For now, nothing needs to be done if there isn't a selected tile that wasn't caught above. Later, if a banner of error or something is shown, that can be added here too
                    std::cout << "Error adding creature" << std::endl;
                }
            }
        }
    }
}

/*
 * TO ADD:
 *
 * CHECK IF THE OCCUPYING CREATURE ON REACHABLE SQUARES ARE ATTACKABLE SPECIFICALLY BY THE CREATURE.
 */
std::vector<Tile> Visualizer::getReachableTiles (Tile creatureTile) {
    //Set the selected tile as the one inputted
    //    glm::ivec2 currentTile = glm::ivec2(creatureTile.x(), creatureTile.y());
    
    if (creatureTile.creature() == nullptr) {
        std::vector<Tile> emptyTileVector;
        return emptyTileVector;
    } else {
        Creature creature = *creatureTile.creature();
        
        std::vector<std::pair<Tile, GLint> > reachedTiles; //This is a vector containing the tiles found so far, along with the energy the creature has at that tile
        
        reachedTiles.push_back(std::pair<Tile, GLint>(creatureTile, creatureTile.creature()->energy()));
        
        //Keep pushing the vector back with new tiles, that the for loop will eventually go through
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            if (reachedTiles[tileIterator].second > 0) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
                
                Tile tile = reachedTiles[tileIterator].first;
                
                //North
                if (tile.y() > 0) {
                    if (this->game.board()->get(tile.x(), tile.y() - 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->game.board()->get(tile.x(), tile.y() - 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x(), tile.y() - 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->game.board()->get(tile.x() - 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->game.board()->get(tile.x() - 1, tile.y()).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x() - 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //South
                if (tile.y() < this->game.board()->height(tile.x()) - 1) {
                    if (this->game.board()->get(tile.x(), tile.y() + 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->game.board()->get(tile.x(), tile.y() + 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x(), tile.y() + 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //West
                if (tile.y() < this->game.board()->width() - 1) {
                    if (this->game.board()->get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->game.board()->get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->game.board()->get(tile.x() + 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
            }
        }
        
        //Now turn the reached tile vector of pairs into a vector of just tiles
        std::vector<Tile> reachedTileReturnVector;
        
        for (GLuint tileIterator = 0; tileIterator < reachedTiles.size(); tileIterator++) {
            reachedTileReturnVector.push_back(reachedTiles[tileIterator].first);
        }
        return reachedTileReturnVector;
    }
}

std::vector<GLuint> Visualizer::getPath(GLuint x, GLuint y, GLuint destinationX, GLuint destinationY) {
    if (x >= this->game.board()->width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (y >= this->game.board()->height(x)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationX >= this->game.board()->width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationY >= this->game.board()->height(destinationX)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (this->game.board()->get(x, y).creature() == nullptr) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (!this->game.board()->get(destinationX, destinationY).passableByCreature(*this->game.board()->get(x, y).creature())) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    }
    
    Creature creature = *this->game.board()->get(x, y).creature();
    
    std::queue<std::vector<std::pair<GLuint, GLuint> > > possiblePaths;
    
    std::vector<std::pair<GLuint, GLuint> > firstTile { std::pair<GLuint, GLuint>(x, y) };
    
    possiblePaths.push(firstTile);
    
    std::vector<std::pair<GLuint, GLuint> > foundPath;
    
    while (possiblePaths.size() > 0) {
        
        std::vector<std::pair<GLuint, GLuint> > path = possiblePaths.front();
        
        if (path.back().first == destinationX && path.back().second == destinationY) {
            foundPath = path;
            break;
        }
        
        if (possiblePaths.front().size() <= creature.energy()) { //If a creature at this spot would be able to continue to move further, expand in the four directions from that tile.
            
            std::pair<GLuint, GLuint> tile = path.back();
            
            //North
            if (tile.second > 0) {
                if (this->game.board()->get(tile.first, tile.second - 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second - 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //East
            if (tile.first > 0) {
                if (this->game.board()->get(tile.first - 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first - 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
            
            //South
            if (tile.second < this->game.board()->height(tile.first) - 1) {
                if (this->game.board()->get(tile.first, tile.second + 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second + 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //West
            if (tile.first < this->game.board()->width() - 1) {
                if (this->game.board()->get(tile.first + 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first + 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
        }
        
        possiblePaths.pop();
    }
    
    std::vector<GLuint> directions;
    
    for (GLuint a = 0; a < foundPath.size(); a++) {
        if (foundPath[a].first == x && foundPath[a].second == y) {
            continue; //First spot is the original location
        }
        
        std::pair<GLuint, GLuint> previousTile = a == 0 ? std::pair<GLuint, GLuint>(x, y) : foundPath[a - 1];
        
        if (foundPath[a].second < previousTile.second && foundPath[a].first == previousTile.first) {
            directions.push_back(NORTH);
        } else if (foundPath[a].first < previousTile.first && foundPath[a].second == previousTile.second) {
            directions.push_back(EAST);
        } else if (foundPath[a].second > previousTile.second && foundPath[a].first == previousTile.first) {
            directions.push_back(SOUTH);
        } else if (foundPath[a].first > previousTile.first && foundPath[a].second == previousTile.second) {
            directions.push_back(WEST);
        }
    }
    
    return directions;
}

//A function GLFW can call when a key event occurs
void Visualizer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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