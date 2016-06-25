//
//  Game.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

//Standard library includes
#include <array>
#include <vector>
#include <exception>

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
#include "GLextensions/shader.hpp"
#include "GLextensions/texture.hpp"
#include "Tile.hpp"


//Preprocessor directives
#define NUMBER_OF_TILES 100
#define INDICES_PER_TILES 3

#define OPEN_TERRAIN 0.0f
#define MOUNTAIN_TERRAIN 1.0f



//An array of booleans representing if, for each key, if that key is pressed
//Declared here so it can work with static function keyCallback. That function needs to be static
bool keys[1024];

class Game {
public:
    //Constructor
    Game(Shader shader, std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    const GLuint windowWidth = 800;
    const GLuint windowHeight = 600;
    
    //Speed of camera motion
    const GLfloat camSpeed = 1.5f;
    
    //Max camera distance from board center
    const GLfloat camMaxDisplacement = 1.0f;
    
    //Public member functions
    void render();
    const void setClearColor(GLfloat red, GLfloat green, GLfloat blue);
    
    //Get functions
    GLFWwindow* window() { return this->gameWindow; }
    const GLfloat timeSinceLastFrame() { return this->deltaTime; }
    
private:
    //Private properties
    std::vector<std::vector<Tile> > board;
    
    //OpenGL and GLFW properties
    GLFWwindow* gameWindow;
    Shader gameShader; //Compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    GLuint VBO; //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    std::array<GLfloat, NUMBER_OF_TILES * INDICES_PER_TILES> vertexData; //Access the contained array within using ".data()"
    
    //Textures
    std::vector<Texture> textures;
    
    //Transformations
    glm::mat4 model; //Makes model isometric
    glm::mat4 view; //Translates camera view
    glm::mat4 projection; //Keeps board scale constant with different window sizes
    
    glm::vec3 cameraCenter;
    
    //Window data
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    glm::vec3 clearColor = glm::vec3(0.0f, 0.0f, 0.0f);

    //Private member functions
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    
    void initWindow();
    void setVertexData();
    void setBuffers();
    void loadTexture(const GLchar* texPath, const GLchar* texName);
    void replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName);
    void presetTransformations(); //Contains matrix transformations to be done on the board. This sets model and projection matrices. Called only once
    
    void moveCamera();
};

#endif /* Game_hpp */
