//
//  Visualizer.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/20/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Visualizer.hpp"

//Constructor
Visualizer::Visualizer(Window* w, std::string vertexPath, std::string geometryPath, std::string fragmentPath, bool* mouseDown, bool* mouseUp, bool* keys) {
//    this->initWindow(); //Create the GLFW window and set the window property
    
    this->isSet = true;
    
    this->window = w;
    
    this->setInterfaces();
    
    this->window->setClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z);
    
    this->gameShader = Shader(vertexPath.c_str(), geometryPath.c_str(), fragmentPath.c_str());
    
    this->font = Font(FONT_PATH);
    
    this->mouseDown = mouseDown;
    this->mouseUp = mouseUp;
    this->keys = keys;
    
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
        this->loadTexture("Resources/archer.png", "archerTex");
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

void Visualizer::set(unsigned int width, unsigned int height) {
    this->boardWidth = width;
    this->boardHeight = height;
    
    this->numberOfTiles = this->boardWidth * this->boardHeight;
    
    this->setVertexData();
    
    this->setBuffers();
    
    this->updateBuffers();
    
    this->camMaxDisplacement = glm::vec2(this->boardWidth * .1, this->boardHeight * .1);
    this->cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
}

//A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
void Visualizer::render() {
    GLfloat currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
    
    //Prepare all of the textures
    for (auto tex = textures.begin(); tex != textures.end(); tex++) {
        tex->use(this->gameShader);
    }
    
    glm::ivec2 framebufferSize = this->window->framebufferSize();
    this->window->setViewport(this->leftInterfaceStats.width, this->bottomInterfaceStats.height, framebufferSize.x - this->leftInterfaceStats.width - this->rightInterfaceStats.width, framebufferSize.y - this->bottomInterfaceStats.height);
    
    this->moveCamera();
    
    this->updateBuffers();
    
    this->gameShader.use();
    
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
        
        interface->render(*this->mouseDown, *this->mouseUp, true);
        
        //Go through the buttons and check if they are pressed, and do any consequential actions
        for (auto button = interface->buttons.begin(); button != interface->buttons.end(); button++) {
            if (button->isPressed()) {
//                this->processButton(button->action);
                this->actions.push_back(button->action);
            }
        }
    }
    
    //Swap buffers so as to properly render without flickering
    this->window->updateScreen();
}

//std::string Visualizer::getClientInfo() {
//    glm::dvec2 mousePos = this->window->cursorPos();
//    glm::ivec2 windowSize = this->window->windowSize();
//    
//    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
//    
//    for (GLuint index = 0; index < this->numberOfTiles; index++) {
//        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
//        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
//    }
//    
//    glm::ivec2 mouseTile = this->mouseTile(mousePos, windowSize, tileCenters);
//    
//    std::string clientInfo = std::to_string(mouseTile.x) + ',' + std::to_string(mouseTile.y) + ',' + (*this->mouseDown ? '1' : '0');
//    
//    clientInfo += ";"; //Separate the previous mouse information from the actions with a semicolon
//    
//    //Add the button actions
//    for (auto str = this->actions.begin(); str != this->actions.end(); str++) {
//        clientInfo += *str + ";"; //Add the action string separated by a semicolon
//        str = this->actions.erase(str);
//    }
//    
//    return clientInfo;
//}

glm::ivec2 Visualizer::getTile(glm::dvec2 pos) {
    glm::ivec2 windowSize = this->window->windowSize();
    
    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
    }
    
    return this->mouseTile(pos, windowSize, tileCenters);
}

glm::ivec2 Visualizer::getTile() {
    return this->getTile(this->window->cursorPos());
}

//void Visualizer::startFrame() {
//    //At the start of each frame, if the mouse has been clicked, then mouseDown will be set to true
//    if (mouseJustPressed) {
//        mouseJustPressed = false;
//        mouseDown = true;
//    }
//}
//
//void Visualizer::endFrame() {
//    //At the end of each frame, if the mouse was clicked before the frame finished, then mouseDown will be set to true, otherwise false
//    if (mouseJustPressed) {
//        mouseJustPressed = false;
//        mouseDown = true;
//    } else {
//        mouseDown = false;
//    }
//}

//Close the window
void Visualizer::closeWindow() {
    this->window->close();
}

//Set the clear color of the screen.
const void Visualizer::setClearColor(GLfloat red, GLfloat green, GLfloat blue) {
    red = red >= 1.0f ? 1.0f : red <= 0.0f ? 0.0f : red;
    green = green >= 1.0f ? 1.0f : green <= 0.0f ? 0.0f : green;
    blue = blue >= 1.0f ? 1.0f : blue <= 0.0f ? 0.0f : blue;
    this->clearColor = glm::vec3(red, green, blue);
}

bool Visualizer::mousePressed() {
    return *this->mouseDown;
}

//Get the time since the previous frame
const GLfloat Visualizer::timeSinceLastFrame() {
    return this->deltaTime;
}

std::map<interfaceType, Interface> Visualizer::getInterfaces() {
    std::map<interfaceType, Interface> interfaces;
    
    Texture tex;
    tex.set("Resources/background.jpg", 29, "tex");
    
    //Left-Side Game UI
    interfaces[default_left].set(&this->textureShader, &this->displayBarShader, this->window, this->leftInterfaceStats.x, this->leftInterfaceStats.y, this->leftInterfaceStats.width, this->leftInterfaceStats.height, tex, default_left);
    
    //Bottom Game UI
    interfaces[default_bottom].set(&this->textureShader, &this->displayBarShader, this->window, this->bottomInterfaceStats.x, this->bottomInterfaceStats.y, this->bottomInterfaceStats.width, this->bottomInterfaceStats.height, tex, default_bottom);
    
    //Right-Side Game UI
    interfaces[default_right].set(&this->textureShader, &this->displayBarShader, this->window, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, tex, default_right);
    
    //Interface for selected creatures
    interfaces[creature].set(&this->textureShader, &this->displayBarShader, this->window, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, tex, creature);
    
    //Interface for selected buildings
    interfaces[building].set(&this->textureShader, &this->displayBarShader, this->window, this->rightInterfaceStats.x, this->rightInterfaceStats.y, this->rightInterfaceStats.width, this->rightInterfaceStats.height, tex, building);
    
    return interfaces;
}

//Private member functions

//Initialize GLFW, GLEW, the key callback function, and the window itself.
void Visualizer::initWindow() {
    //Make a window object. May throw an error, which will be thrown through this function
    this->window->init(this->windowWidth, this->windowHeight, "Game", false, true);
    
    //Tell OpenGL window information
    glm::vec2 framebufferSize = this->window->framebufferSize();
    this->window->setViewport(this->leftInterfaceStats.width, this->bottomInterfaceStats.height, framebufferSize.x - this->leftInterfaceStats.width - this->rightInterfaceStats.width, framebufferSize.y - this->bottomInterfaceStats.height); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfacecs
    
    //Set key callback function
//    this->window.setKeyCallback(this->keyCallback);
//    
//    //Set mouse button click callback function
//    this->window.setMouseButtonCallback(this->mouseButtonCallback);
}

void Visualizer::setInterfaces() {
    glm::ivec2 framebufferSize = this->window->framebufferSize();
    
    this->leftInterfaceStats = interfaceStat(0.0, 0.0, framebufferSize.x / 6.0, framebufferSize.y);
    this->bottomInterfaceStats = interfaceStat(framebufferSize.x * 1.0 / 6.0, 0.0, framebufferSize.x * 2.0 / 3.0, framebufferSize.y / 4.0);
    this->rightInterfaceStats = interfaceStat(framebufferSize.x * 5.0 / 6.0, 0.0, framebufferSize.x / 6.0, framebufferSize.y);
    
    this->textureShader = Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag");
    
    this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
}

void Visualizer::setVertexData() {
    GLfloat pointDistance = 2.0 / BOARD_SIZE_CONSTANT;
    
    glm::vec2 locationOfFirstPoint;
    locationOfFirstPoint.x += ((float)this->boardWidth / 2.0) * (pointDistance);
    locationOfFirstPoint.y += ((float)this->boardWidth / 2.0) * (pointDistance);
    
//    glm::vec2 locationOfFirstPoint = glm::vec2(1.0, 1.0);
//    locationOfFirstPoint.x -= pointDistance / 2.0; //Half of the distance between points is before the first point and after the last
//    locationOfFirstPoint.y -= pointDistance / 2.0;
    
    std::vector<GLfloat> vertexDataVec;
    
    for (GLuint x = 0; x < this->boardWidth; x++) {
        for (GLuint y = 0; y < this->boardHeight; y++) {
            //Sets the point location based on the location in the board and on the modifiers above.
            vertexDataVec.push_back(locationOfFirstPoint.x - (x * pointDistance));
            vertexDataVec.push_back(locationOfFirstPoint.y - (y * pointDistance));
        }
    }
    
    this->vertexData = vertexDataVec;
}

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
    
    GLfloat vertices[2 * this->numberOfTiles];
    
    for (int a = 0; a < this->numberOfTiles; a++) {
        vertices[2 * a] = this->vertexData[2 * a];
        vertices[(2 * a) + 1] = this->vertexData[(2 * a) + 1];
    }
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

//A function to update all of the buffers that need to be updated. Should be called every frame.
void Visualizer::updateBuffers() {
    GLint terrains[this->numberOfTiles];
    GLint creatures[3 * this->numberOfTiles];
    GLfloat colors[3 * this->numberOfTiles];
    GLint damages[this->numberOfTiles];
    GLfloat offsets[this->numberOfTiles];
    GLint buildings[2 * this->numberOfTiles];
    
    for (int a = 0; a < this->numberOfTiles; a++) {
        terrains[a] = this->terrainData[a];
        
        creatures[3 * a] = this->creatureData[3 * a];
        creatures[(3 * a) + 1] = this->creatureData[(3 * a) + 1];
        creatures[(3 * a) + 2] = this->creatureData[(3 * a) + 2];
        
        colors[3 * a] = this->colorData[3 * a];
        colors[(3 * a) + 1] = this->colorData[(3 * a) + 1];
        colors[(3 * a) + 2] = this->colorData[(3 * a) + 2];
        
        damages[a] = this->damageData[a];
        
        offsets[a] = this->offsetData[a];
        
        buildings[2 * a] = this->buildingData[2 * a];
        buildings[(2 * a) + 1] = this->buildingData[(2 * a) + 1];
    }
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terrains), terrains, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->creatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(creatures), creatures, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(3);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->damageVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(damages), damages, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(4);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(5);
    
    //Bind the VBO with the data
    glBindBuffer(GL_ARRAY_BUFFER, this->buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buildings), buildings, GL_DYNAMIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLint), (GLvoid*)0);
    glEnableVertexAttribArray(6);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //And finally we unbind the VAO so we don't do any accidental misconfiguring
    glBindVertexArray(0);
}

//Loads a texture into the back of the vector of texture objects. Only works up to 32 times. Throws an error if there are already 32 textures.
void Visualizer::loadTexture(const GLchar* texPath, const GLchar* texName) {
    if (textures.size() <= 32) {
        Texture tex;
        tex.set(texPath, (GLuint)textures.size(), texName);
        textures.push_back(tex);
    } else {
        throw std::range_error("32 textures already loaded.");
    }
}

//Replaces the designated spot in the vector of texture objects with a new texture. Throws an error if the desired index is out of vector range.
void Visualizer::replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName) {
    if (texIndex < textures.size()) {
        Texture tex;
        tex.set(texPath, (GLuint)textures.size(), texName);
        textures[texIndex] = tex;
    } else {
        throw std::range_error("No texture loaded in that spot.");
    }
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
    
    //Counteract the 45º rotation of model
    this->rectRotation = glm::rotate(this->rectRotation, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //Counteract the y-scaling down of model
    this->rectRotation = glm::scale(this->rectRotation, glm::vec3(1.0f, 2.0f, 1.0f));
    
    //Send the recrRotation matrix to the shader
    this->gameShader.uniformMat4("rectRotation", this->rectRotation);
    
    //Orthographic (non-3D projection) added so that different window sizes don't distort the scale
    this->projection = glm::ortho((GLfloat)windowWidth / (GLfloat)windowHeight * -1.0f, (GLfloat)windowWidth / (GLfloat)windowHeight * 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    //Send the projection matrix to the shader
    this->gameShader.uniformMat4("ortho", this->projection);
}

void Visualizer::renderDamageText() {
    std::vector<glm::vec4> tileCenters; //Representing the center point of all of the map squares
    
    for (GLuint index = 0; index < this->numberOfTiles; index++) {
        //Set the vector as the transformed point, using the location data from vertexData. VertexData is twice the length, so we access it by multiplying the index by 2 (and sometimes adding 1)
        tileCenters.push_back(this->projection * this->view * this->model * glm::vec4(this->vertexData[2 * index], this->vertexData[(2 * index) + 1], 0.0f, 1.0f));
    }
    
    //Goes through existence times and updates them based on glfwGetTime()
    for (GLuint tile = 0; tile < this->numberOfTiles; tile++) {
        
        if (this->damageData[tile] != 0) { //Don't show the damage if it is not new. 0 is used to represent null or old values

            glm::vec2 damageTileCoords = tileCenters[tile];
            
            damageTileCoords.x += 1.0;
            damageTileCoords.x /= 2.0;
            damageTileCoords.y += 1.0;
            damageTileCoords.y /= 2.0;
            
            glm::vec2 fontSize = this->font.getSize(std::to_string(this->damageData[tile]), 1.0);
            
            glm::vec2 viewportSize = this->window->viewportSize();
            
            this->font.render(std::to_string(this->damageData[tile]), damageTileCoords.x * viewportSize.x - (fontSize.x / 2), damageTileCoords.y * viewportSize.y - (fontSize.y / 2), 1.0, glm::vec3(1.0, 1.0, 1.0), viewportSize.x, viewportSize.y);
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
    
    /*
     The board is rotated 45 degrees counterclockwise, so the height, which was previously up-down, now becomes upper left-lower right, and the opposite for the width
     If we say camMaxDisplacement = (w, h):
     The width hits the vertical side, so after rotation the vertical side becomes the one with slope = -1.
     A point on that line should be (w / sqrt(2), w / sqrt(2)). Since the distance from the line to the center is w, another point on it should be (0, w * sqrt(2)).
     We need to check that the point is below that line. To find the equation of that line, we can use the form
        y = slope ( x - h ) + k, where (h, k) is a point on that line.
     We also need to check it is above another line parallel but below. That should be the same line except a point on it would be (0, -w * sqrt(2)) instead.
     We can do the same for h, except that the line we are going to was the top / bottom, so it will now have slope +1. A point on the line would be (h / sqrt(2), h / sqrt(2)), which would mean also (h * sqrt(2), 0) would be.
     
     We will also have to half all vertical distances to scale properly with the board
     */
    
    float root2 = sqrtf(2.0);
    
    //For the y coordinate, we use y = slope ( x - h ) + k
    
    //If it is not below the line with slope -1 and (0, w * sqrt(2)), it is too far.
    if (this->cameraCenter.y > -( this->cameraCenter.x ) + (this->camMaxDisplacement.x + root2)) {
        this->cameraCenter.y = -( this->cameraCenter.x ) + (this->camMaxDisplacement.x + root2);
    }
    
    //If it is not above the line with slope -1 and (0, -w * sqrt(2)), it is too far.
    if (this->cameraCenter.y < -( this->cameraCenter.x ) - (this->camMaxDisplacement.x + root2)) {
        this->cameraCenter.y = -( this->cameraCenter.x ) - (this->camMaxDisplacement.x + root2);
    }
    
    //If it is not above the line with slope 1 and (h * sqrt(2), 0), it is too far.
    if (this->cameraCenter.y < (this->cameraCenter.x - (this->camMaxDisplacement.x + root2))) {
        this->cameraCenter.y = (this->cameraCenter.x - (this->camMaxDisplacement.x + root2));
    }
    
    //If it is not below the line with slope 1 and (-h * sqrt(2), 0), it is too far.
    if (this->cameraCenter.y > (this->cameraCenter.x + (this->camMaxDisplacement.x + root2))) {
        this->cameraCenter.y = (this->cameraCenter.x + (this->camMaxDisplacement.x + root2));
    }
    
    //For the x coordinate, we use an altered form: x = ( ( y - k ) / slope ) + h
    
    //If it is not left of the line with slope -1 and (0, w * sqrt(2)), it is too far.
    if (this->cameraCenter.x > -(this->cameraCenter.y - this->camMaxDisplacement.x * root2)) {
        this->cameraCenter.x = -(this->cameraCenter.y - this->camMaxDisplacement.x * root2);
    }
    
    //If it is not right of the line with slope -1 and (0, -w * sqrt(2)), it is too far.
    if (this->cameraCenter.x < -(this->cameraCenter.y + this->camMaxDisplacement.x * root2)) {
        this->cameraCenter.x = -(this->cameraCenter.y + this->camMaxDisplacement.x * root2);
    }
    
    //If it is not left of the line with slope +1 and (h * sqrt(2), 0), it is too far.
    if (this->cameraCenter.x > this->cameraCenter.y + this->camMaxDisplacement.y * root2) {
        this->cameraCenter.x = this->cameraCenter.y + this->camMaxDisplacement.y * root2;
    }
    
    //If it is not right of the line with slope +1 and (-h * sqrt(2), 0), it is too far.
    if (this->cameraCenter.x < this->cameraCenter.y - this->camMaxDisplacement.y * root2) {
        this->cameraCenter.x = this->cameraCenter.y - this->camMaxDisplacement.y * root2;
    }
    
//    //Guarantees that the camera won't move too far from the board center
//    if (this->cameraCenter.x > this->camMaxDisplacement.x)
//        this->cameraCenter.x = this->camMaxDisplacement.x;
//    if (this->cameraCenter.x < -this->camMaxDisplacement.x)
//        this->cameraCenter.x = -this->camMaxDisplacement.x;
//    if (this->cameraCenter.y > this->camMaxDisplacement.y)
//        this->cameraCenter.y = this->camMaxDisplacement.y;
//    if (this->cameraCenter.y < -this->camMaxDisplacement.y)
//        this->cameraCenter.y = -this->camMaxDisplacement.y;
}

//void Visualizer::processButton(std::string action) {
//    if (action == "return_to_menu") {
//        
////        this->showSettings = true;
//        
//    } else {
//        this->actions.push_back(action);
//    }
//}

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
    
    //The distance from one point to the horizontal point and the vertical point:
    
    //The points diagonally above and below each vertex become horizontal and vertical after rotation. To find them, find the point below the vertex and add one and subtract one.
    
    if (this->boardWidth < 2) { //In case finding the distances (just below) would cause a bad access
        throw std::length_error("Board too small; Board size: (" + std::to_string(this->boardWidth) + ", " + std::to_string(this->boardHeight) + ")");
    }
    
    GLfloat distance1 = Visualizer::getDistance(tileCenters[0], tileCenters[0 + this->boardHeight + 1]); //Diagonal down and to the right
    GLfloat distance2 = Visualizer::getDistance(tileCenters[1], tileCenters[1 + this->boardHeight - 1]); //Diagonal down and to the left
    
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
        
        //h stays the same
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
    
    tileIndexVec.x = (int)(tileIndex / this->boardHeight); //The x index in the 2D vector
    
    tileIndexVec.y = tileIndex - (this->boardHeight * tileIndexVec.x); //The y index in the 2D vector
    
    return tileIndexVec;
}

Visualizer::~Visualizer() {
    if (this->isSet) {
        try {
            glDeleteVertexArrays(1, &this->VAO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete VAO. Error thrown with call of glDeleteVertexArrays(1, &this->VAO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->vertexVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete vertexVBO. Error thrown with call of glDeleteBuffers(1, &this->vertexVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->terrainVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete terrainVBO. Error thrown with call of glDeleteBuffers(1, &this->terrainVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->creatureVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete creauterVBO. Error thrown with call of glDeleteBuffers(1, &this->creatureVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->colorVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete colorVBO. Error thrown with call of glDeleteBuffers(1, &this->colorVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->damageVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete damageVBO. Error thrown with call of glDeleteBuffers(1, &this->damageVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->offsetVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete offsetVBO. Error thrown with call of glDeleteBuffers(1, &this->offsetVBO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->buildingVBO);
        } catch(...) {
            std::cout << "~Visualizer(): Unable to properly delete buildingVBO. Error thrown with call of glDeleteBuffers(1, &this->buildingVBO)." << std::endl;
        }
    }
}
