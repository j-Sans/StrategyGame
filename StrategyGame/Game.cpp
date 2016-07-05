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

//A boolean representing if the active tile should be set. This boolean is set in the mouse button callback function
bool activateTile = false;

//Constructor without geometry shader
Game::Game(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : gameBoard(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true, true, true); //Set all of the data arrays with information from the board
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
    try {
        this->loadTexture("Resources/numbers.png", "numbersTex");
    } catch (std::exception e) {
        std::cout << "Error loading numbers texture: " << e.what();
    }
    
    this->presetTransformations();
}

//Constructor with geometry shader
Game::Game(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board) : gameBoard(board) {
    this->initWindow(); //Create the GLFW window and set the window property
    this->setData(true, true, true, true, true, true); //Set the data arrays with information from the board
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
    try {
        this->loadTexture("Resources/numbers.png", "numbersTex");
    } catch (std::exception e) {
        std::cout << "Error loading numbers texture: " << e.what();
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
    
    //Prepare all of the textures
    for (auto tex = textures.begin(); tex != textures.end(); tex++) {
        tex->use(this->gameShader);
    }
    
    //If the mouse was clicked, set the color of the tile that was clicked
    if (activateTile) {
        this->updateTileStyle();
        
        activateTile = false;
    }
    
    //Update creature offset, so as to implement animation. Done before updating creature buffer so that updates are rendered
    this->updateCreatureOffset();
    
    //Update the creatures
    this->updateCreatureBuffer();
    
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
    glDeleteBuffers(1, &this->colorVBO);
    glDeleteBuffers(1, &this->damageVBO);
    glDeleteBuffers(1, &this->offsetVBO);
    
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
    
    //Set mouse button click callback function
    glfwSetMouseButtonCallback(this->gameWindow, this->mouseButtonCallback);
}

//Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
void Game::setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData) {
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
    GLint directions[numberOfIndices];
    GLfloat colors[3 * numberOfIndices];
    
    index = 0;
    
    for (GLuint x = 0; x < this->gameBoard.width(); x++) {
        for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
            if (index < numberOfIndices) { //Checks to make sure no data outside of the array is accessed.
                if (setTerrainData)
                    //Gets the terrain of the tile
                    terrains[index] = this->gameBoard.get(x, y).terrain();
                
                if (setCreatureData) {
                    //Gets the creature on the tile
                    creatures[index] = this->gameBoard.get(x, y).creatureType();
                    
                    //Gets the direction if there is a creature there
                    if (this->gameBoard.get(x, y).creature() != nullptr)
                        directions[index] = this->gameBoard.get(x, y).creature()->direction();
                    else
                        directions[index] = NORTH;
                }
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
        if (setCreatureData) {
            this->creatureData[2 * a] = creatures[a];
            
            this->creatureData[(2 * a) + 1] = directions[a];
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
void Game::setBuffers() {
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
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
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
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
}

//A function to update the creature VBO. Should be called every frame
void Game::updateCreatureBuffer() {
    //Update creature data array
    this->setData(false, false, true, false, false, false);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->creatureData), this->creatureData, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

//A function to update the color VBO. Should be called every frame
void Game::updateColorBuffer() {
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
void Game::updateDamageBuffer() {
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

void Game::updateCreatureOffset() {
    GLfloat displacement = this->creatureSpeed * deltaTime;
    
    //Goes through all tiles and continues moving any that are moving
    for (GLuint tile = 0; tile < NUMBER_OF_TILES; tile++) {
        
        GLuint direction = this->creatureData[(2 * tile) + 1];
        
        glm::ivec2 boardLoc;
        boardLoc.x = tile / this->gameBoard.width();
        boardLoc.y = tile - (this->gameBoard.width() * boardLoc.x);
        
        if (direction == NORTH || direction == WEST) {
            //These two directions cause the creature to move up, visually, so they stay at the current tile until they reach the above one. If they moved tiles first, then the previous tile, which is lower, would be drawn on top
            
            if (this->offsetData[tile] > 0.0) {
                this->offsetData[tile] += displacement;
            }
            
            //At 0.4, it has reached the next tile
            if (this->offsetData[tile] > 0.4) {
                this->offsetData[tile] = 0.0;
                
                try {
                    this->gameBoard.moveCreatureByDirection(boardLoc.x, boardLoc.y, direction);
                } catch (std::exception e) {
                    //The creature can't move in that direction for some reason...
                    //This should never happen because the clicked location was an open adjacent tile, so there should be no reason it can't be moved there
                }
            }
        } else if (direction == SOUTH || direction == EAST) {
            GLuint index = tile;
            if (direction == SOUTH) {
                index += this->gameBoard.width(); //One row below
            } else if (direction == EAST) {
                index += 1; //One tile further
            }
            
            if (tile < NUMBER_OF_TILES) {
            
                //These two directions cause the creature to move udown, visually, so they move to the lower tile first. If they moved tiles after, then the new tile, which is lower, would be drawn on top
                
                //The displacement goes negative. In the shader, this is added to 0.4, so it gets closer to 0 as it gets closer to the new tile.
                if (this->offsetData[tile] < 0.0) {
                    this->offsetData[tile] += displacement;
                }
                
                //At 0.4, it has reached the next tile
                if (this->offsetData[tile] >= 0.0) {
                    this->offsetData[tile] = 0.0;
                    
                    //The creature is not moved here. It should have already been moved in the function that deals with mouse clicks.
                }
            }
        }
        
        
        /*
        
        if (this->offsetData[tile] > 0) {
            this->offsetData[tile] += displacement;
        }
        
        if (this->offsetData[tile] > 0.4) { //At a distance of 0.4, it has reached the neighboring tile, so we just move the creature
            this->offsetData[tile] = 0;
            
            GLuint direction = creatureData[(2 * tile) + 1];
            
            glm::ivec2 boardLoc;
            boardLoc.x = tile / this->gameBoard.width();
            boardLoc.y = tile - (this->gameBoard.width() * boardLoc.x);
            
            if (direction == NORTH) {
                try {
                    this->gameBoard.moveCreatureByDirection(boardLoc.x, boardLoc.y, NORTH);
                } catch(std::exception e) {
                    //Board is already furthest north, or there is a creature north. Either way, can't move north
                }
            } else if (direction == EAST) {
                try {
                    this->gameBoard.moveCreatureByDirection(boardLoc.x, boardLoc.y, EAST);
                } catch(std::exception e) {
                    //Board is already furthest east, or there is a creature east. Either way, can't move east
                }
            } else if (direction == SOUTH) {
                try {
                    this->gameBoard.moveCreatureByDirection(boardLoc.x, boardLoc.y, SOUTH);
                } catch(std::exception e) {
                    //Board is already furthest south, or there is a creature south. Either way, can't move south
                }
            } else if (direction == WEST) {
                try {
                    this->gameBoard.moveCreatureByDirection(boardLoc.x, boardLoc.y, WEST);
                } catch(std::exception e) {
                    //Board is already furthest west, or there is a creature west. Either way, can't move west
                }
            }
        }
         */
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

void Game::updateTileStyle() {
    glm::ivec2 mousePos;
    
    try {
        mousePos = mouseTile(); //Fails if mouse is outside of the board
        
        //Reset the tile (and others) if the current tile is clicked again
        if (mousePos == this->selectedTile) {
            
            //Goes through all tiles and sets them to regular
            for (GLuint x = 0; x < this->gameBoard.width(); x++) {
                for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                    this->gameBoard.setStyle(x, y, Regular);
                }
            }
            
            //Set selectedTile to null results
            this->selectedTile = glm::ivec2(-1, -1);
        }
        
        //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
        else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == Regular) {
            
            //Reset all tiles (this one is highlighted after)
            for (GLuint x = 0; x < this->gameBoard.width(); x++) {
                for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                    this->gameBoard.setStyle(x, y, Regular);
                }
            }
            
            //Select this new tile
            this->gameBoard.setStyle(mousePos.x, mousePos.y, Selected);
            
            //If the selected tile is a creature, highlight adjacent tiles and update the creature's direction
            if (this->gameBoard.get(mousePos.x, mousePos.y).creature() != nullptr) {
                
                Creature creature = *this->gameBoard.get(mousePos.x, mousePos.y).creature();
                
                try { //North tile
                    if (this->gameBoard.get(mousePos.x, mousePos.y - 1).passableByCreature(creature))
                        this->gameBoard.setStyle(mousePos.x, mousePos.y - 1, OpenAdj);
                    else if (this->gameBoard.get(mousePos.x, mousePos.y - 1).creature() != nullptr)
                        this->gameBoard.setStyle(mousePos.x, mousePos.y - 1, AttackableAdj);
                } catch (std::exception e) {
                    //No northern tile
                }
                
                try { //West tile
                    if (this->gameBoard.get(mousePos.x - 1, mousePos.y).passableByCreature(creature))
                        this->gameBoard.setStyle(mousePos.x - 1, mousePos.y, OpenAdj);
                    else if (this->gameBoard.get(mousePos.x - 1, mousePos.y).creature() != nullptr)
                        this->gameBoard.setStyle(mousePos.x - 1, mousePos.y, AttackableAdj);
                } catch (std::exception e) {
                    //No western tile
                }
                
                try { //South tile
                    if (this->gameBoard.get(mousePos.x, mousePos.y + 1).passableByCreature(creature))
                        this->gameBoard.setStyle(mousePos.x, mousePos.y + 1, OpenAdj);
                    else if (this->gameBoard.get(mousePos.x, mousePos.y + 1).creature() != nullptr)
                        this->gameBoard.setStyle(mousePos.x, mousePos.y + 1, AttackableAdj);
                } catch (std::exception e) {
                    //No southern tile
                }
                
                try { //East tile
                    if (this->gameBoard.get(mousePos.x + 1, mousePos.y).passableByCreature(creature))
                        this->gameBoard.setStyle(mousePos.x + 1, mousePos.y, OpenAdj);
                    else if (this->gameBoard.get(mousePos.x + 1, mousePos.y).creature() != nullptr)
                        this->gameBoard.setStyle(mousePos.x + 1, mousePos.y, AttackableAdj);
                } catch (std::exception e) {
                    //No eastern tile
                }
            }
            
            this->selectedTile = mousePos;
        }
        
        //Movement
        else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == OpenAdj) {
            
            //Get the direction of the click
            int direction = 0;
            
            if (mousePos.x == this->selectedTile.x && mousePos.y == this->selectedTile.y - 1) {
                direction = NORTH;
            }
            if (mousePos.x == this->selectedTile.x + 1 && mousePos.y == this->selectedTile.y) {
                direction = EAST;
            }
            if (mousePos.x == this->selectedTile.x && mousePos.y == this->selectedTile.y + 1) {
                direction = SOUTH;
            }
            if (mousePos.x == this->selectedTile.x - 1 && mousePos.y == this->selectedTile.y) {
                direction = WEST;
            }
            
            //Set the direction that was found at the selected creature
            this->gameBoard.setDirection(this->selectedTile.x, this->selectedTile.y, direction);
            
            //If the tile is going to be moving up (visually on the screen) slowly move the tile from the previous location to the new one
            //For these directions, the creature is moved after, in the function that updates the offset data
            if (direction == NORTH || direction == WEST)
                this->offsetData[(this->selectedTile.x * this->gameBoard.width()) + this->selectedTile.y] += (this->creatureSpeed * this->deltaTime);
            
            //If it's going down, instead move it to the next square and slowly move it from that spot. This keeps it from being drawn under the tile it's going to
            //For these directions, the creature is moved here, and then the offset is slowly updated to follow
            if (direction == SOUTH || direction == EAST) {
                GLuint tile; //The location in the data array
                
                if (direction == SOUTH) {
                    tile = (this->selectedTile.x * this->gameBoard.width()) + (this->selectedTile.y + 1); //One row below
                } else if (direction == EAST) {
                    tile = ((this->selectedTile.x + 1) * this->gameBoard.width()) + this->selectedTile.y; //One tile further
                }
                
                if (tile < NUMBER_OF_TILES) {
                    this->offsetData[tile] = -0.4; //-= (this->creatureSpeed * this->deltaTime);
                    
                    try {
                        this->gameBoard.moveCreatureByDirection(this->selectedTile.x, this->selectedTile.y, direction);
                    } catch (std::exception e) {
                        //The creature can't move in that direction for some reason...
                        //This should never happen because the clicked location was an open adjacent tile, so there should be no reason it can't be moved there
                    }
                }
            }
            
            //Reset all tiles
            for (GLuint x = 0; x < this->gameBoard.width(); x++) {
                for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                    this->gameBoard.setStyle(x, y, Regular);
                }
            }
            
            this->selectedTile = glm::ivec2(-1, -1);
        }
        
        //Attacking
        else if (this->gameBoard.get(mousePos.x, mousePos.y).style() == AttackableAdj) {
            
            int attackDamage = 0;
            int defendDamage = 0;
            
            glm::ivec2 attacker = glm::ivec2(this->selectedTile.x, this->selectedTile.y);
            glm::ivec2 defender = glm::ivec2(mousePos.x, mousePos.y);
            
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
            
            this->selectedTile = glm::ivec2(-1, -1);
        }
        
    } catch (std::exception e) {
        //The mouse was outside of the board
        // (Or mouseTile() returned an out of bounds index, but this shouldn't happen)
        
        //Reset all tiles
        for (GLuint x = 0; x < this->gameBoard.width(); x++) {
            for (GLuint y = 0; y < this->gameBoard.height(x); y++) {
                this->gameBoard.setStyle(x, y, Regular);
            }
        }
    }
}

//Calculates the tile that the mouse is over
glm::ivec2 Game::mouseTile() {
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
    
    GLfloat distance1 = Game::getDistance(tileCenters[0], tileCenters[0 + BOARD_WIDTH + 1]); //Diagonal down and to the right
    GLfloat distance2 = Game::getDistance(tileCenters[1], tileCenters[1 + BOARD_WIDTH - 1]); //Diagonal down and to the left
    
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
        GLfloat k = center.y - (verticalDistance / 2);
        
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
        k = center.y + (verticalDistance / 2);
        
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
    
    if (tileIndex == -1)
        throw std::range_error("Mouse outside of board");
    
    glm::ivec2 tileIndexVec;
    
    tileIndexVec.x = (int)(tileIndex / BOARD_WIDTH); //The x index in the 2D vector
    
    tileIndexVec.y = tileIndex - (BOARD_WIDTH * tileIndexVec.x); //The y index in the 2D vector
    
    return tileIndexVec;
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
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        activateTile = true;
    }
}