//
//  Visualizer.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/20/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Visualizer_hpp
#define Visualizer_hpp

#include "Macros.h"

//Standard library includes
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <exception>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//SOIL: Sets up texture loading
#include <SOIL/SOIL.h>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Local includes
#include "Shader.hpp"
#include "ClientSocket.hpp"
#include "Texture.hpp"
#include "Font.hpp"

#include "Interface.hpp"
#include "Window.hpp"

//For when no tile is being selected
#define NO_SELECTION glm::ivec2(-1, -1)

//For when no tile is being selected
#define INTERFACE_BOX_SELECTION glm::ivec2(-2, -2)

enum BoardInfoDataTypes {
    TerrainData,
    CreatureData,
    ColorData,
    DamageData,
    OffsetData,
    BuildingData,
};

//A simple struct to hold the data of interfaces
struct interfaceStat {
    GLuint x;
    GLuint y;
    GLuint width;
    GLuint height;
    
    interfaceStat() {} //So that it can be constructed at a later point within the visualizer class
    
    interfaceStat(GLuint interfaceX, GLuint interfaceY, GLuint interfaceWidth, GLuint interfaceHeight) {
        this->x = interfaceX;
        this->y = interfaceY;
        this->width = interfaceWidth;
        this->height = interfaceHeight;
    }
};

class Visualizer {
public:
    //Constructors
    
    /*!
     * A class that sets up and renders the screen, abstracting from the GLFW and OpenGL details.
     *
     * @param vertexPath A string which is the path to the text file that contains vertex shader GLSL code.
     * @param geometryPath A string which is the path to the text file that contains geometry shader GLSL code.
     * @param fragmentPath A string which is the path to the text file that contains fragment shader GLSL code.
     */
    Visualizer(std::string vertexPath, std::string geometryPath, std::string fragmentPath);
    
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
    
    //Public member functions
    
    /*!
     * Distance formula using Pythagorean Theorem
     */
    static GLfloat getDistance(glm::vec2 point1, glm::vec2 point2);
    
    //Game info, to be set from Client
    unsigned int playerNum;
    unsigned int activePlayer;
    
    Window window;
    
    //Array data to be sent to respective VBO's
    std::vector<GLfloat> vertexData; //[NUMBER_OF_TILES * INDICES_PER_TILES];
    std::vector<GLint> terrainData; //[NUMBER_OF_TILES];
    std::vector<GLint> creatureData; //[3 * NUMBER_OF_TILES]; //1 value for the creature type, 1 for the direction, 1 for the controller
    std::vector<GLfloat> colorData; //[3 * NUMBER_OF_TILES]; //3 values, one for each RGB
    std::vector<GLint> damageData; //[NUMBER_OF_TILES]; //The damage to be displayed on this tile. If it is 0, nothing will be displayed.
    std::vector<GLfloat> offsetData; //[NUMBER_OF_TILES]; //For animation, the offset from the point in the given direction
    std::vector<GLint> buildingData; //[2 * NUMBER_OF_TILES]; //1 value for the building type, 1 for the controller
    
    /*!
     * A function that sets the initial information of the visualizer
     *
     * @param width An unsigned int representing the number of tiles along the x axis on the board.
     * @param height An unsigned int representing the number of tiles along the y axis on the board.
     * @param initialInfo An std::map of std::strings with enum type BoardInfoDataTypes (see Visualizer.hpp) as the keys, for initially setting openGL buffers.
     */
    void set(unsigned int width, unsigned int height, std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec);
    
    /*!
     * A function that sets the view matrix based on camera position and renders everything on the screen. Should be called once per frame.
     *
     * @param boardInfo An std::map of std::strings with enum type BoardInfoDataTypes (see Visualizer.hpp) as the keys, for updating openGL buffers.
     */
//    void render(std::map<BoardInfoDataTypes, std::string> boardInfo);
    void render(/* std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec*/);
    
    /*!
     * A function to return a string conatining the following.
     *  mouse_x_position,mouse_y_position,t
     *  mouse_x_position,mouse_y_position,f
     * The first one if the mouse is down, the second if the mouse is not down.
     * This string should be sent to the host.
     *
     * @return The string to send to the host as described above.
     */
    std::string getClientInfo();
    
    /*!
     * @return The coordinates of the tile where the mouse is located.
     */
    glm::ivec2 getMouseTile();
    
    /*!
     * A function that should be called at the beginning of each frame from the larger class, in this case client.
     * If not, errors involving the mouse being pressed will occur.
     */
    void startFrame();
    
    /*!
     * A function that should be called at the end of each frame from the larger class, in this case client.
     * If not, errors involving the mouse being pressed will occur.
     */
    void endFrame();
    
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
     * @return Whether the mouse is has been pressed.
     */
    bool mousePressed();
    
    /*!
     * @return A GLfloat representing the time (in seconds) since the last frame.
     */
    const GLfloat timeSinceLastFrame();
    
private:
    //Private properties
    
    bool isSet = false;
    
    //Board info
    unsigned int boardWidth;
    unsigned int boardHeight;
    unsigned int numberOfTiles;
    
    //OpenGL and GLFW properties
//    GLFWwindow* gameWindow;
    Shader gameShader; //Compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint vertexVBO;
    GLuint terrainVBO;
    GLuint creatureVBO;
    GLuint colorVBO;
    GLuint damageVBO;
    GLuint offsetVBO;
    GLuint buildingVBO;
    
    //Textures
    std::vector<Texture> textures;
    
    //Font
    Font font;
    
    //Information for host
    std::list<std::string> actionsForClientInfo;
    
    //Transformations
    glm::mat4 model; //Makes model isometric
    glm::mat4 view; //Translates camera view
    glm::mat4 projection; //Keeps board scale constant with different window sizes
    
    glm::mat4 rectRotation;
    
    glm::vec3 cameraCenter;
    GLfloat camMaxDisplacement;
    
    //Window data
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    glm::vec3 clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::ivec2 viewportSize;
    glm::ivec2 selectedTile = NO_SELECTION;
    
    //Interfaces
    std::map<interfaceType, Interface> interfaces;
    Shader interfaceShader;
    Shader buttonShader;
    Shader displayBarShader;
    
    Interface *leftInterface;
    Interface *bottomInterface;
    Interface *rightInterface;
    
    interfaceStat leftInterfaceStats;
    interfaceStat bottomInterfaceStats;
    interfaceStat rightInterfaceStats;
    interfaceStat settingsMenuStats;
    
    //Settings menu
    Box darkenBox; //A box to render to darken the entire screen
    bool showSettings = false; //A boolean representing whether or not to display the settings menu
    
    
    //Private member functions
    
    /*!
     * Initialize GLFW, GLEW, the key callback function, and the window itself.
     */
    void initWindow();
    
//    /*!
//     * Set the data for the VBO's for vertices, terrains, and creatures. Information is taken from the board.
//     *
//     * @param setVertexData A boolean indicating whether to update the vertex data array.
//     * @param setTerrainData A boolean indicating whether to update the terrain data array.
//     * @param setCreatureData A boolean indicating whether to update the creature data array.
//     * @param setColorData A boolean indicating whether to update the color data array.
//     * @param setDamageData A boolean indicating whether to update the damage data array.
//     * @param setOffsetData A boolean indicating whether to update the offset data array.
//     * @param setBuildingData A boolean indicating whether to update the building data array.
//     */
//    void setData(bool setVertexData, bool setTerrainData, bool setCreatureData, bool setColorData, bool setDamageData, bool setOffsetData, bool setBuildingData);
    
    /*!
     * Initialize OpenGL buffers with the object's vertex data.
     *
     * @param boardInfo An std::string representing the information directly received from the server, through the socket.
     */
//    void setBuffers(std::map<BoardInfoDataTypes, std::string> boardInfo);
    void setBuffers(std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec);
    
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
     * A function to update all of the buffers that need to be updated. Should be called every frame.
     *
     * @param boardInfo An std::string representing the information directly received from the server, through the socket.
     */
//    void updateBuffers(std::map<BoardInfoDataTypes, std::string> boardInfo);
    void updateBuffers(std::vector<int> terrainDataVec, std::vector<int> creatureDataVec, std::vector<float> colorDataVec, std::vector<int> damageDataVec, std::vector<float> offsetDataVec, std::vector<int> buildingDataVec);
    
    /*!
     * Set the correct interfaces to render based on the selected tile. Should be called every frame.
     */
    void updateInterfaces();
    
    /*!
     * A function to render the damage text. Should be called every frame after displaying the board.
     */
    void renderDamageText();
    
    /*!
     * A function that should be called every frame and alters the global cameraCenter vector to move the camera based on arrowkey inputs.
     */
    void moveCamera();
    
    /*!
     * A function to deal with button clicks.
     *
     * @param action An std::string representing the button action to do.
     */
    void processButton(std::string action);
    
    /*!
     * A function to render a display menu interface in the center of the screen. This can be useful as an "esc" settings menu (when escape is clicked). This also darkens the rest of the screen.
     *
     * @param mouseUp A bool representing if the mouse was just released.
     * @param mouseDown A bool representing if the mouse is currently clicking.
     */
    void renderSettingsMenu(bool mouseUp, bool mouseDown);
    
    /*!
     * A function to calculate the tile closest to the mouse location at any given point in time.
     * (-1, -1) is returned if the selection was outside of the board.
     * Possible errors include if the board size is below 2x2, because calculations require a board size at least that large.
     *
     * @param mousePos A glm::vec2 of the cursor position, in screen coordinates. Can be obtained from glfwGetCursorPos.
     * @param windowSize A glm::ivec2 representing the window size, in screen coordinates. Can be obtained from glfwGetWindowSize.
     * @param tileCenters An std::vector of glm::vec4 (should be length NUMBER_OF_TILES) representing the center of each tile, after transformations. This can be gotten by manipulating the vertex data and multiplying it with the transformation matrices.
     *
     * @return The tile indices in the board, the 2D vector. In the form of a glm vector of 2 ints (glm::ivec2).
     */
    glm::ivec2 mouseTile(glm::vec2 mousePos, glm::ivec2 windowSize, std::vector<glm::vec4> tileCenters);
    
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

#endif /* Visualizer_hpp */
