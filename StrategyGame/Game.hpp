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
#include <math.h>
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
#include "Player.hpp"

#include "Interface.hpp"


//Preprocessor directives
#define BOARD_WIDTH 12
#define NUMBER_OF_TILES BOARD_WIDTH * BOARD_WIDTH
#define INDICES_PER_TILES 2
#define NUMBER_OF_PLAYERS 2

#define NO_SELECTION glm::ivec2(-1, -1)
#define INTERFACE_BOX_SELECTION glm::ivec2(-2, -2)

#define FULL_SCREEN

class Game {
public:
    //Constructors
    
    /*!
     * A class that sets up and renders the screen, abstracting from the GLFW and OpenGL details.
     *
     * @param vertexPath A c-string which is the path to the text file that contains vertex shader GLSL code.
     * @param fragmentPath A c-string which is the path to the text file that contains fragment shader GLSL code.
     * @param board A 2D vector of Tiles representing the game board.
     */
    Game(const GLchar* vertexPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board);
    
    /*!
     * A class that sets up and renders the screen, abstracting from the GLFW and OpenGL details.
     *
     * @param vertexPath A c-string which is the path to the text file that contains vertex shader GLSL code.
     * @param geometryPath An optional c-string which is the path to the text file that contains geometry shader GLSL code. This parameter is not necessary.
     * @param fragmentPath A c-string which is the path to the text file that contains fragment shader GLSL code.
     * @param board A 2D vector of Tiles representing the game board.
     */
    Game(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    
    /*!
     * The set width of the window. Due to high screen resolution, the final window may not actually be this many pixels wide.
     */
    const GLuint windowWidth = 800;
    
    /*!
     * The set height of the window. Due to high screen resolution, the final window may not actually be this many pixels high.
     */
    const GLuint windowHeight = 600;
    
    /*!
     * The speed at which the camera pans across the board.
     */
    const GLfloat camSpeed = 1.5f;
    
    /*!
     * A limiting factor preventing the camera from moving too far off screen.
     */
    const GLfloat camMaxDisplacement = BOARD_WIDTH / 10.0f;
    
    /*!
     * The speed that the creature moves when it is animated.
     */
    const GLfloat movementAnimationSpeed = 1.5f;
    
    /*!
     * Time before deletion of damage boxes, in seconds.
     */
    const GLfloat damageBoxTime = 3.0f;
    
    /*!
     * The game board, containing a 2D vector of Tile objects.
     */
    Board gameBoard;
    
    //Public member functions
    
    /*!
     * Distance formula using Pythagorean Theorem
     */
    static GLfloat getDistance(glm::vec2 point1, glm::vec2 point2);
    
    /*!
     * A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
     */
    void render();
    
    /*!
     * A function that closes the window. Should be called within game loop to exit the loop.
     */
    void closeWindow();
    
    /*!
     * A function that helps to terminates the program and deallocates various resources. Should be called after closeWindow(), after leaving the game loop.
     */
    void terminate();
    
    /*!
     * Set the clear color of the screen.
     *
     * @param red The red value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     * @param green The green value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     * @param blue The blue value of the color, on a scale of 0.0 to 1.0. Outside this scale will be mapped to 0.0 or 1.0.
     */
    const void setClearColor(GLfloat red, GLfloat green, GLfloat blue);
    
    //Get functions
    
    /*!
     * @return A pointer to the GLFWwindow object.
     */
    GLFWwindow* window();
    
    /*!
     * @return A GLfloat representing the time (in seconds) since the last frame.
     */
    const GLfloat timeSinceLastFrame();
    
private:
    //Private properties
    
    //OpenGL and GLFW properties
    GLFWwindow* gameWindow;
    Shader gameShader; //Compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint vertexVBO;
    GLuint terrainVBO;
    GLuint creatureVBO;
    GLuint colorVBO;
    GLuint damageVBO;
    GLuint offsetVBO;
    
    //Array data to be sent to respective VBO's
    GLfloat vertexData[NUMBER_OF_TILES * INDICES_PER_TILES];
    GLint terrainData[NUMBER_OF_TILES];
    GLint creatureData[3 * NUMBER_OF_TILES]; //1 value for the creature type, 1 for the direction, 1 for the controller
    GLfloat colorData[3 * NUMBER_OF_TILES]; //3 values, one for each RGB
    GLint damageData[NUMBER_OF_TILES];
    GLfloat offsetData[NUMBER_OF_TILES]; //For animation, the offset from the point in the given direction
    
    GLint existenceTimeForDamageData[NUMBER_OF_TILES]; //For each damage data, represents the amount of time that box has existed for
    
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
    
    //Interfaces
    std::vector<Interface> interfaces;
    Shader interfaceShader;
    Shader buttonShader;
    
    //Board data
    glm::ivec2 selectedTile = glm::ivec2(-1, -1);
    
    //Player and turn data
    Player players[NUMBER_OF_PLAYERS];
    GLuint activePlayer = 0;
    GLuint turn = 1;
    

    //Private member functions
    
    /*!
     * Initialize GLFW, GLEW, the key callback function, and the window itself.
     */
    void initWindow();
    
    /*!
     * Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
     *
     * @param setVertexData A boolean indicating whether to update the vertex data array
     * @param setTerrainData A boolean indicating whether to update the terrain data array
     * @param setCreatureData A boolean indicating whether to update the creature data array
     * @param setColorData A boolean indicating whether to update the color data array
     * @param setDamageData A boolean indicating whether to update the damage data array
     * @param setOffsetData A boolean indicating whether to update the offset data array
     */
    void setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData);
    
    /*!
     * Initialize OpenGL buffers with the object's vertex data.
     */
    void setBuffers();
    
    /*!
     * Initialize the interface
     */
    void setInterface();
    
    /*!
     * Loads a texture into the back of the vector of texture objects. Only works up to 32 times. Throws an error if there are already 32 textures.
     * An error will be thrown if there are already 32 textures loaded.
     *
     * @param texPath A string representing the path to the texture image.
     * @param texName A string representing the uniform name of the texture.
     */
    void loadTexture(const GLchar* texPath, const GLchar* texName);
    
    /*!
     * Replaces the designated spot in the vector of texture objects with a new texture. Throws an error if the desired index is out of vector range.
     * An error will be thrown if there is no index texIndex within the vector of textures.
     *
     * @param texPath A string representing the path to the texture image.
     * @param texIndex The index in the vector of textures to be replaced.
     * @param texName A string representing the uniform name of the texture.
     */
    void replaceTexture(const GLchar* texPath, GLuint texIndex, const GLchar* texName);
    
    /*!
     * A function that sets matrix transformations to be done on the board. This sets the model matrix to rotate the board by 45º and then make it seem tilted away from the viewer. That is done by scaling so that the horizontal diagnal is double the vertical one. The projection matrix is also added, using glm::ortho, so that a non-3D orthographic projection is achieved. It is made so that regardless of window dimensions, the scaling on the board is always constant.
     */
    void presetTransformations();
    
    /*!
     * A function to update both the creature VBO and the offset VBO. Should be called every frame.
     */
    void updateCreatures();
    
    /*!
     * A function to update the color VBO. Should be called every frame.
     */
    void updateColorBuffer();
    
    /*!
     * A function to update the damage VBO. Should be called every frame.
     */
    void updateDamageBuffer();
    
    /*!
     * A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
     */
    void moveCamera();
    
    /*!
     * A function that alters the style and colors of tiles based on mouse clicks.
     */
    void updateSelected();
    
    /*!
     * A function to deal with button clicks.
     *
     * @param action An std::string representing the button action to do.
     */
    void processButton(std::string action);
    
    /*!
     * A function to move to the next player's turn.
     */
    void incrementActivePlayer();
    
    /*!
     * A function that moves the creature at the designated location in the designated direction.
     *
     * @param x The x location of the creature in the board.
     * @param y The y location of the creature in the board.
     * @direction The direction to move the creature in.
     *
     * @return Whether the creature was moved.
     */
    bool moveAdjacent(GLuint x, GLuint y, int direction);
    
    /*!
     * A function to calculate the tile closest to the mouse location at any given point in time.
     * (-1, -1) is returned if the selection was outside of the board.
     * Possible errors include if the board size is below 2x2, because calculations require a board size at least that large.
     *
     * @return The tile indices in the board, the 2D vector. In the form of a glm vector of 2 ints (glm::ivec2).
     */
    glm::ivec2 mouseTile();
    
    /*!
     * A function that gets all of the tiles that a creature can reach.
     *
     * @param creature The tile containing the specified creature that is going to be moving.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be reached.
     */
    std::vector<Tile> getReachableTiles(Tile creatureTile);
    
    /*!
     * A function GLFW can call when a key event occurs.
     *
     * @param window The GLFWwindow object.
     * @param key The macro that will represent the key pressed
     * @param action The macro that represents if the key is being pressed, released, etc...
     * @param mods The macro representing which, if any, modes are activated, such as shift, command, etc...
     */
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    
    /*!
     * A function GLFW can call when a mouse click occurs.
     *
     * @param window The GLFWwindow object.
     * @param button The macro that will represent the mouse button pressed
     * @param action The macro that represents if the key is being pressed, released, etc...
     * @param mode The macro representing which, if any, modes are activated, such as shift, command, etc...
     */
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
};

#endif /* Game_hpp */
