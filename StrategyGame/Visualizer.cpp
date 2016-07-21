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

//A boolean representing if the active tile should be set. This boolean is set in the mouse button callback function
bool mouseDown = false;

//Constructor without geometry shader
Visualizer::Visualizer(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : game(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true, true, true); //Set all of the data arrays with information from the board
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    this->gameShader = Shader(vertexPath, fragmentPath);
    
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
        this->loadTexture("Resources/numbers.png", "numbersTex");
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
    this->setData(true, true, true, true, true, true); //Set the data arrays with information from the board
    this->setBuffers(); //Set up all of the OpenGL buffers with the vertex data
    
    this->gameShader = Shader(vertexPath, geometryPath, fragmentPath);
    
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
        this->loadTexture("Resources/numbers.png", "numbersTex");
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
        
        //This function deals with mouse clicks. If the mouse was clicked in an interface box, mouseDown is returned to true so that the buttons can check if there is any click
        this->updateSelected();
    }
    
    //Update the creatures and their offsets
    this->updateCreatures();
    
    //Update tile colors
    this->updateColorBuffer();
    
    //Update damage boxes
    this->updateDamageBuffer();
    
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
    
    //Go through the interfaces and render them
    for (GLuint a = 0; a < interfaces.size(); a++) {
        this->interfaces[a].render(mouseDown); //This renders the interface and its buttons
        
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
    
    //Set key callback function
    glfwSetKeyCallback(this->gameWindow, this->keyCallback);
    
    //Set mouse button click callback function
    glfwSetMouseButtonCallback(this->gameWindow, this->mouseButtonCallback);
}

//Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
void Visualizer::setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData) {
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
            this->existenceTimeForDamageData[a] = 0;
        }
        if (setOffsetData) {
            this->offsetData[a] = 0;
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
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Creature VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Color VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorData), this->colorData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    //Damage VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData), this->damageData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    //Offset VBO:
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->offsetData), this->offsetData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
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

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//Review this function
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

//A function to update the creature VBO. Should be called every frame
void Visualizer::updateCreatures() {
    //Set the offset VBO
    
    //Update creature data array
    this->setData(false, false, true, false, false, false);
    
    //Goes through all tiles and continues moving any that are moving
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
        
        glm::ivec2 boardLoc;
        boardLoc.x = tile / this->game.board()->width();
        boardLoc.y = tile - (this->game.board()->width() * boardLoc.x);
        
        Creature* creature = this->game.board()->get(boardLoc.x, boardLoc.y).creature();
        
        if (creature != nullptr) {
            
            glm::ivec2 creatureLoc = boardLoc;
            
            GLuint direction = creature->direction();
            
            if (direction == NORTH || direction == EAST) {
                //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
                
                //If the creature is in the process of moving currently, continue to move it
                creature->incrementOffset(this->deltaTime);
                
                //Correctly set this tile's offset data
                this->offsetData[tile] = creature->offset();
                
                if (creature->readyToMove()) {
                    if (direction == NORTH) {
                        if (this->game.board()->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                            creatureLoc.y -= 1;
                        }
                    } else if (direction == EAST) {
                        if (this->game.board()->moveCreatureByDirection(creatureLoc.x, creatureLoc.y, direction)) {
                            creatureLoc.x -= 1;
                        }
                    }
                    
                }
            } else if (direction == SOUTH || direction == WEST) {
                
                creature->incrementOffset(this->deltaTime);
                
                //Correctly set this tile's offset data
                this->offsetData[tile] = creature->offset();
            }
            
            if (creature->directions.size() > 0 && creature->offset() == 0.0) {
                
                //Get the new direction that the creature will be travelling in.
                GLuint newDirection = creature->directions.front();
                
                //Now that this direction is being dealt with, we can get rid of it from the directions left for the creature to go in.
                creature->directions.pop();
                
                this->moveAdjacent(creatureLoc.x, creatureLoc.y, newDirection);
            }
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
    //Position
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Now set the creature VBO
    
    //Update creature data array. This is done again so that if the offset was adjusted and the creature moved, that is reflected in the creature VBO and rendered. Otherwise, the creature would momentarily blink and flash back to its previous location
    this->setData(false, false, true, false, false, false);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
    
    //Update creature data array
    this->setData(false, false, true, false, false, false);
}

//A function to update the color VBO. Should be called every frame
void Visualizer::updateColorBuffer() {
    //Update creature data array
    this->setData(false, false, false, true, false, false);
    
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

//A function to update the damage VBO. Should be called every frame
void Visualizer::updateDamageBuffer() {
    //Goes through existence times and updates them based on glfwGetTime()
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
        if (this->damageData[tile] != 0) {
            if (glfwGetTime() - this->existenceTimeForDamageData[tile] > this->damageBoxTime) {
                //If the damage box has existed for long enough
                this->damageData[tile] = 0;
                this->existenceTimeForDamageData[tile] = 0;
            }
        }
    }
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->damageData), this->damageData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
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

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//Review this function
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Visualizer::updateSelected() {
    glm::ivec2 mousePos;
    
    mousePos = mouseTile();
    
    if (mousePos == INTERFACE_BOX_SELECTION) {
        //Don't alter the selected tile if the interface box has been clicked
        
        //Make mouseDown true again so the interface box can check the mouse click location
        mouseDown = true;
        
    } else if (mousePos == NO_SELECTION) {
        //Reset all tiles if the mouse clicked out of the screen
        for (GLuint x = 0; x < this->game.board()->width(); x++) {
            for (GLuint y = 0; y < this->game.board()->height(x); y++) {
                this->game.board()->setStyle(x, y, Regular);
            }
        }
    }
    
    //Reset the tile (and others) if the current tile is clicked again
    else if (mousePos == this->selectedTile) {
        
        //Goes through all tiles and sets them to regular
        for (GLuint x = 0; x < this->game.board()->width(); x++) {
            for (GLuint y = 0; y < this->game.board()->height(x); y++) {
                this->game.board()->setStyle(x, y, Regular);
            }
        }
        
        //Set selectedTile to null results
        this->selectedTile = NO_SELECTION;
    }
    
    //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
    else if (this->game.board()->get(mousePos.x, mousePos.y).style() == Regular) {
        
        //Reset all tiles (this one is highlighted after)
        for (GLuint x = 0; x < this->game.board()->width(); x++) {
            for (GLuint y = 0; y < this->game.board()->height(x); y++) {
                this->game.board()->setStyle(x, y, Regular);
            }
        }
        
        //Select this new tile
        this->game.board()->setStyle(mousePos.x, mousePos.y, Selected);
        
        //If the selected tile is a creature, highlight reachable tiles and update the creature's direction
        
        if (this->game.board()->get(mousePos.x, mousePos.y).creature() != nullptr && this->game.board()->get(mousePos.x, mousePos.y).creature()->controller() == activePlayer) {
            std::vector<Tile> reachableTiles = getReachableTiles(this->game.board()->get(mousePos.x, mousePos.y));
            
            Creature creature = *this->game.board()->get(mousePos.x, mousePos.y).creature();
            for (GLuint a = 0; a < reachableTiles.size(); a++) {
                if (this->game.board()->get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(creature)) {
                    this->game.board()->setStyle(reachableTiles[a].x(), reachableTiles[a].y(), Reachable);
                } else if (this->game.board()->get(reachableTiles[a].x(), reachableTiles[a].y()).creature() != nullptr && this->game.board()->get(reachableTiles[a].x(), reachableTiles[a].y()).creature()->controller() != this->activePlayer) {
                    
                    //Only set the tile to be attackable if it is within the creature's range
                    if (this->game.board()->tileDistances(mousePos.x, mousePos.y, reachableTiles[a].x(), reachableTiles[a].y()) <= creature.range())
                        this->game.board()->setStyle(reachableTiles[a].x(), reachableTiles[a].y(), AttackableAdj);
                }
            }
        }
        
        this->selectedTile = mousePos;
    }
    
    //Movement
    else if (this->game.board()->get(mousePos.x, mousePos.y).style() == Reachable) {
        
        std::vector<GLuint> directions = this->getPath(this->selectedTile.x, this->selectedTile.y, mousePos.x, mousePos.y);
        
        for (GLuint a = 0; a < directions.size(); a++) {
            this->game.board()->get(this->selectedTile.x, this->selectedTile.y).creature()->directions.push(directions[a]);
            if (a == 0)
                this->game.board()->setDirection(this->selectedTile.x, this->selectedTile.y, directions[a]);
        }
        
        //Reset all tiles
        for (GLuint x = 0; x < this->gameBoard.width(); x++) {
            for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
        
        this->selectedTile = NO_SELECTION;
    }
    
    //Attacking
    else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == AttackableAdj) {
        
        glm::ivec2 attacker = glm::ivec2(this->selectedTile.x, this->selectedTile.y);
        glm::ivec2 defender = glm::ivec2(mousePos.x, mousePos.y);
        
        if (this->gameBoard.tileDistances(attacker.x, attacker.y, defender.x, defender.y) <= this->gameBoard.get(attacker.x, attacker.y).creature()->range()) {
            
            int attackDamage = 0;
            int defendDamage = 0;
            
            this->gameBoard.attack(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
            
            //Set the damage data on the defending square equal to damage dealt by the attacker
            this->damageData[(defender.x * this->gameBoard.width()) + defender.y] = attackDamage;
            this->existenceTimeForDamageData[(defender.x * this->gameBoard.width()) + defender.y] = glfwGetTime();
            
            //Set the damage data on the attacking square equal to damage dealt by the defender
            this->damageData[(attacker.x * this->gameBoard.width()) + attacker.y] = defendDamage;
            this->existenceTimeForDamageData[(attacker.x * this->gameBoard.width()) + attacker.y] = glfwGetTime();
            
            
            //Reset all tiles
            for (GLuint x = 0; x < this->gameBoard.width(); x++) {
                for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                    this->gameBoard.setStyle(x, y, Regular);
                }
            }
            
            this->selectedTile = NO_SELECTION;
        }
    }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//Review this function
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

bool Visualizer::moveAdjacent(GLuint x, GLuint y, int direction) {
    //Return false if there is no creature at the designated spot to move
    if (this->gameBoard.get(x, y).creature() == nullptr)
        return false;
    
    //Check if move goes beyond map
    int newX, newY;
    
    if (direction == NORTH) {
        newX = x;
        newY = y - 1;
        
        if (newY < 0)
            return false;
    } else if (direction == WEST) {
        newX = x + 1;
        newY = y;
        
        if (newX >= this->gameBoard.width())
            return false;
    } else if (direction == SOUTH) {
        newX = x;
        newY = y + 1;
        
        if (newY >= this->gameBoard.height(x))
            return false;
    } else if (direction == EAST) {
        newX = x - 1;
        newY = y;
        
        if (newX < 0)
            return false;
    }
    
    //Passable Check
    if (!this->gameBoard.get(newX, newY).passableByCreature(*this->gameBoard.get(x, y).creature())) {
        return false;
    }
    
    //Set the direction that was found at the selected creature
    this->gameBoard.setDirection(x, y, direction);
    
    //If the tile is going to be moving up (visually on the screen) slowly move the tile from the previous location to the new one
    //For these directions, the creature is moved after, in the function that updates the offset data
    if (direction == NORTH || direction == EAST)
        this->gameBoard.get(x, y).creature()->initiateMovementOffset(this->deltaTime);
    
    //If it's going down, instead move it to the next square and slowly move it from that spot. This keeps it from being drawn under the tile it's going to
    //For these directions, the creature is moved here, and then the offset is slowly updated to follow
    if (direction == SOUTH || direction == WEST) {
        GLuint tile; //The location in the data array
        
        if (direction == SOUTH) {
            tile = (x * this->gameBoard.width()) + (y + 1); //One row below
        } else if (direction == WEST) {
            tile = ((x + 1) * this->gameBoard.width()) + y; //One tile further
        }
        
        if (tile < NUMBER_OF_TILES) {
            this->gameBoard.get(x, y).creature()->initiateMovementOffset(this->deltaTime);
            
            this->gameBoard.moveCreatureByDirection(x, y, direction);
        }
    }
    return true;
}

void Visualizer::processButton(std::string action) {
    //Process the button indicating to move to the next turn
    if (action == "next turn") {
        
        //Iterate through the entire board and reset style and energy.
        for (GLuint x = 0; x < this->gameBoard.width(); x++) {
            for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
                Creature* creature = this->gameBoard.get(x, y).creature();
                if (creature != nullptr)
                    creature->resetEnergy();
            }
        }
        
        this->incrementActivePlayer();
        std::cout << std::endl << std::endl << "--------------------" << std::endl << "Player " << this->activePlayer + 1 << "'s turn" << std::endl << std::endl;
        //Increment the turn if a full player cycle has occurred
        if (activePlayer == 0)
            turn++;
    } else if (action.find("creature") != std::string::npos) { //Basically if the string action contains "creature", the button makes a creature
        if (this->selectedTile != NO_SELECTION && this->selectedTile != INTERFACE_BOX_SELECTION && !this->gameBoard.get(this->selectedTile.x, this->selectedTile.y).occupied()) {
            
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
            
            Creature newCreature(race, values[0], values[1], values[2], values[3], values[4], values[5], direction, this->activePlayer);
            
            if (this->gameBoard.get(this->selectedTile.x, this->selectedTile.y).passableByCreature(newCreature)) {
                try {
                    this->gameBoard.setCreature(this->selectedTile.x, this->selectedTile.y, newCreature);
                    
                    //Reset all tiles to be unselected now that the creature has been added
                    for (GLuint x = 0; x < this->gameBoard.width(); x++) {
                        for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                            this->gameBoard.setStyle(x, y, Regular);
                        }
                    }
                    
                    this->selectedTile = NO_SELECTION;
                    
                } catch (std::exception) {
                    //For now, nothing needs to be done if there isn't a selected tile that wasn't caught above. Later, if a banner of error or something is shown, that can be added here too
                    std::cout << "Error adding creature" << std::endl;
                }
            }
        }
    }
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//Review this function
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void Visualizer::incrementActivePlayer() {
    this->activePlayer++;
    
    if (this->activePlayer >= NUMBER_OF_PLAYERS)
        this->activePlayer = 0;
}



//Calculates the tile that the mouse is over
glm::ivec2 Visualizer::mouseTile() {
    GLint tileIndex = -1; //The tile index where the mouse was clicked. Initialized as -1 to mean no index found
    
    int width, height;
    double xPos, yPos;
    
    glfwGetCursorPos(this->gameWindow, &xPos, &yPos);
    
    glfwGetWindowSize(this->gameWindow, &width, &height);
    
    //Get the mousePos to be in the same coordinates as the vertexData (-1 to 1)
    
    
    
    //If x is in the last sixth or the first sixth, ignore the click because the interface boxes were clicked
    if (xPos > (width * 5.0 / 6.0) || xPos < (width / 6.0))
        return INTERFACE_BOX_SELECTION;
    
    //Only the middle 2/3 of the screen is the board, so make the start of that section 0
    xPos -= (width / 6.0);
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    xPos *= (3.0 / 2.0);
    
    //Do the same for y, except that only the bottom 1/4 of the screen is not part of the board
    if (yPos > (height * 3.0 / 4.0))
        return INTERFACE_BOX_SELECTION;
    
    //Then make it 2/3 of the size to dilate it with the board's dilation
    yPos *= (4.0 / 3.0);
    
    //Make mousePos between 0 and 1 by dividing the position by the maximum position (width or height)
    xPos /= width;
    yPos /= height;
    
    //Now make it 0 to 2 by doubling it
    xPos *= 2.0f;
    yPos *= 2.0f;
    
    //Now subtract 1 to get it between -1 and 1
    xPos -= 1.0f;
    yPos -= 1.0f;
    
    //So that -1 is the bottom of the screen, not the top
    yPos = -yPos;
    
    glm::vec2 mousePos(xPos, yPos);
    
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
    
    GLfloat distance1 = Visualizer::getDistance(tileCenters[0], tileCenters[0 + BOARD_WIDTH + 1]); //Diagonal down and to the right
    GLfloat distance2 = Visualizer::getDistance(tileCenters[1], tileCenters[1 + BOARD_WIDTH - 1]); //Diagonal down and to the left
    
    //Distance horizontally is double the distance of the vertical one because it was compressed vertically.
    //The horizontal distance is the max of the above distances, and the vertical distance the minimum
    
    GLfloat verticalDistance = fminf(distance1, distance2);
    GLfloat horizontalDistance = fmaxf(distance1, distance2);
    
    //For every point, check if it is within the boundaries of the respective diamond's bounds, by finding the 4 bounding lines of that rectange
    
    GLfloat slope = verticalDistance / horizontalDistance; // = rise / run
    
    //Using line equation:
    // y = slope ( x - h ) + k
    //Where (h,k) is a point on the line
    
    for (GLuint index = 0; index < NUMBER_OF_TILES; index++) {
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
    
    tileIndexVec.x = (int)(tileIndex / BOARD_WIDTH); //The x index in the 2D vector
    
    tileIndexVec.y = tileIndex - (BOARD_WIDTH * tileIndexVec.x); //The y index in the 2D vector
    
    return tileIndexVec;
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
                    if (this->gameBoard.get(tile.x(), tile.y() - 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() - 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() - 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //East
                if (tile.x() > 0) {
                    if (this->gameBoard.get(tile.x() - 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() - 1, tile.y()).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() - 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //South
                if (tile.y() < this->gameBoard.height(tile.x()) - 1) {
                    if (this->gameBoard.get(tile.x(), tile.y() + 1).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x(), tile.y() + 1).occupied()) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x(), tile.y() + 1), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
                    }
                }
                
                //West
                if (tile.y() < this->gameBoard.width() - 1) {
                    if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), reachedTiles[tileIterator].second - 1)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have - 1.
                    } else if (this->gameBoard.get(tile.x() + 1, tile.y()).passableByCreature(creature)) {
                        reachedTiles.push_back(std::pair<Tile, GLint>(this->gameBoard.get(tile.x() + 1, tile.y()), 0)); //Add the found tile to the reached tiles, along with the value of the energy the creature would have after attacking, which is 0.
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
    if (x >= this->gameBoard.width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (y >= this->gameBoard.height(x)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationX >= this->gameBoard.width()) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (destinationY >= this->gameBoard.height(destinationX)) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (this->gameBoard.get(x, y).creature() == nullptr) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    } else if (!this->gameBoard.get(destinationX, destinationY).passableByCreature(*this->gameBoard.get(x, y).creature())) {
        std::vector<GLuint> emptyVector;
        return emptyVector;
    }
    
    Creature creature = *this->gameBoard.get(x, y).creature();
    
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
                if (this->gameBoard.get(tile.first, tile.second - 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second - 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //East
            if (tile.first > 0) {
                if (this->gameBoard.get(tile.first - 1, tile.second).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first - 1, tile.second));
                    possiblePaths.push(nextPath);
                }
            }
            
            //South
            if (tile.second < this->gameBoard.height(tile.first) - 1) {
                if (this->gameBoard.get(tile.first, tile.second + 1).passableByCreature(creature)) {
                    std::vector<std::pair<GLuint, GLuint> > nextPath = path;
                    nextPath.push_back(std::pair<GLuint, GLuint>(tile.first, tile.second + 1));
                    possiblePaths.push(nextPath);
                }
            }
            
            //West
            if (tile.first < this->gameBoard.width() - 1) {
                if (this->gameBoard.get(tile.first + 1, tile.second).passableByCreature(creature)) {
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
}