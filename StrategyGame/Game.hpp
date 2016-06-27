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
//#include "GLextensions/shader.hpp"
//#include "GLextensions/texture.hpp"
//#include <myGL/shader.h>
//#include "texture.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Tile.hpp"
#include "Board.hpp"


//Preprocessor directives
#define NUMBER_OF_TILES 100
#define INDICES_PER_TILES 3

#define OPEN_TERRAIN 0.0f
#define MOUNTAIN_TERRAIN 1.0f



/**
 * A class that sets up and renders the screen, abstracting from the GLFW and OpenGL details.
 *
 * @param vertexPath A c-string which is the path to the text file that contains vertex shader GLSL code.
 * @param geometryPath An optional c-string which is the path to the text file that contains geometry shader GLSL code. This parameter is not necessary.
 * @param fragmentPath A c-string which is the path to the text file that contains fragment shader GLSL code.
 * @param board A 2D vector of Tiles representing the game board.
 */
class Game {
public:
    //Constructors
    Game(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board);
    
    Game(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    
    /**
     * The set width of the window. Due to high screen resolution, the final window may not actually be this many pixels wide.
     */
    const GLuint windowWidth = 800;
    
    /**
     * The set height of the window. Due to high screen resolution, the final window may not actually be this many pixels high.
     */
    const GLuint windowHeight = 600;
    
    /**
     * The speed at which the camera pans across the board.
     */
    const GLfloat camSpeed = 1.5f;
    
    /**
     * A limiting factor preventing the camera from moving too far off screen.
     */
    const GLfloat camMaxDisplacement = 1.0f;
    
    //Public member functions
    
    /**
     * A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
     */
    void render();
    
    /**
     * A function that closes the window. Should be called within game loop to exit the loop.
     */
    void closeWindow();
    
    /**
     * A function that helps to terminates the program and deallocates various resources. Should be called after closeWindow(), after leaving the game loop.
     */
    void terminate();
    
    /**
     * Set the clear color of the screen.
     *
     * @param red The red value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     * @param green The green value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     * @param blue The blue value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     */
    const void setClearColor(GLfloat red, GLfloat green, GLfloat blue);
    
    //Get functions
    
    /**
     * @return A pointer to the GLFWwindow object.
     */
    GLFWwindow* window();
    
    /**
     * @return A GLfloat representing the time (in seconds) since the last frame.
     */
    const GLfloat timeSinceLastFrame();
    
private:
    //Private properties
    Board gameBoard;
    
    //OpenGL and GLFW properties
    GLFWwindow* gameWindow;
    Shader gameShader; //Compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    GLuint VBO; //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLfloat vertexData[NUMBER_OF_TILES * INDICES_PER_TILES];
    
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
    
    /**
     * Initialize GLFW, GLEW, the key callback function, and the window itself.
     */
    void initWindow();
    
    /**
     * Set the vertex data as a std::array in the object. Eventually will be made to get the data from the board or from a file, but is hardcoded for now.
     */
    void setVertexData();
    
    /**
     * Initialize OpenGL buffers with the object's vertex data.
     */
    void setBuffers();
    
    /**
     * Loads a texture into the back of the vector of texture objects. Only works up to 32 times. Throws an error if there are already 32 textures.
     * An error will be thrown if there are already 32 textures loaded.
     *
     * @param texPath A string representing the path to the texture image.
     * @param texName A string representing the uniform name of the texture.
     */
    void loadTexture(const GLchar* texPath, const GLchar* texName);
    
    /**
     * Replaces the designated spot in the vector of texture objects with a new texture. Throws an error if the desired index is out of vector range.
     * An error will be thrown if there is no index texIndex within the vector of textures.
     *
     * @param texPath A string representing the path to the texture image.
     * @param texIndex The index in the vector of textures to be replaced.
     * @param texName A string representing the uniform name of the texture.
     */
    void replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName);
    
    /**
     * A function that sets matrix transformations to be done on the board. This sets the model matrix to rotate the board by 45º and then make it seem tilted away from the viewer. That is done by scaling so that the horizontal diagnal is double the vertical one. The projection matrix is also added, using glm::ortho, so that a non-3D orthographic projection is achieved. It is made so that regardless of window dimensions, the scaling on the board is always constant.
     */
    void presetTransformations();
    
    /**
     * A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
     */
    void moveCamera();
    
    /**
     * A function GLFW can call when a key event occurs.
     *
     * @param window The GLFWwindow object.
     * @param key The macro that will represent the key pressed
     * @param action The macro that represents if the key is being pressed, released, etc...
     * @param mode The macro representing which, if any, modes are activated, such as shift, command, etc...
     */
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
};

#endif /* Game_hpp */
