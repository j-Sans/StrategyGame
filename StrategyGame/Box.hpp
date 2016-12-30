//
//  Box.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 8/20/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Box_hpp
#define Box_hpp

#include "Macros.h"

//Standard library includes
#include <string>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//GLM: OpenGL mathematics for vectors here
#include <GLM/glm.hpp>

//Local includes
#include "Shader.hpp"
#include "Font.hpp"
#include "Window.hpp"

enum DisplayBoxType {
    //Creature info boxes
    creature_attack,
    creature_range,
    creature_vision,
    creature_race,
    
    //Other
    other,
};

class Box {
public:
    //Constructors
    
    //Default constructor. Don't use this, it is only to allow box objects to be declared without causing an error with memory.cpp
    Box();
    
    /*!
     * A class representing a box on an interface block. This creates a box with the default button color.
     *
     * @param shader A compiled shader for rendering this box.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the box in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the box in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the box in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the box in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     * @param boxText An std::string representing the text to display on the box as its name.
     * @param type A box type indicating what kind of box this should be updated as. Use the type other for no updating. Otherwise, see Box.hpp.
     */
    Box(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string boxText, DisplayBoxType type);
    
    /*!
     * A class representing a box on an interface block. This creates a box with the given button color
     *
     * @param shader A compiled shader for rendering this box.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the box in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the box in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the box in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the box in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     * @param color An optional glm::vec4 representing the color to set as the box color.
     * @param boxText An std::string representing the text to display on the box as its name.
     * @param type A box type indicating what kind of box this should be updated as. Use the type other for no updating. Otherwise, see Box.hpp.
     */
    Box(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, glm::vec4 color, std::string boxText, DisplayBoxType type);
    
    //Static properties
    constexpr static float defaultColor = 0.33; //Set in constructor
    
    //Public properties
    
    std::string text; //A string to have as the title of the box, what will be displayed.
    
    //Public member functions
    
    /*!
     * A function to render the box. Should be called within the interface's rendering function.
     */
    void render();
    
    //Public get functions
    
    /*!
     * @return The type of the box, for updating the box text.
     */
    DisplayBoxType type();
    
    /*!
     * @return The color of the box.
     */
    glm::vec4 color();
    
private:
    //Box properties
    DisplayBoxType boxType; //A displayBoxType to represent the type of the box, so that its can be updated
    glm::vec4 boxColor;
    
    //OpenGL and GLFW properties
    Window* boxWindow;
    Shader boxShader; //A compiled shader to render this box
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint boxVBO;
    GLuint colorVBO;
    
    Font font;
    
    //Viewport information
    GLfloat lowerLeftX;
    GLfloat lowerLeftY;
    GLfloat boxWidth;
    GLfloat boxHeight;
    
    GLfloat interfaceBoxLowerLeftX;
    GLfloat interfaceBoxLowerLeftY;
    GLfloat interfaceBoxWidth;
    GLfloat interfaceBoxHeight;
    
    //Private member functions
    
};

#endif /* Box_hpp */
