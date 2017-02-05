//
//  DisplayBar.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 8/16/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef DisplayBar_hpp
#define DisplayBar_hpp

#include "Macros.h"

//Standard library includes

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

enum DisplayBarType {
    HealthBar,
    EnergyBar,
};

class DisplayBar {
public:
    //Constructors
    
    //Default constructor. Don't use this, it is only to allow DisplayBar objects to be declared without causing an error with memory.cpp
    DisplayBar();
    
    /*!
     * A class representing a display bar on an interface block.
     *
     * @param shader A pointer to a compiled shader for rendering this display bar.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the bar in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the bar in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the bar in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the bar in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     * @param maxVal A float representing the max value that this bar can hold.
     * @param barText An std::string representing the text to display on the button as its name.
     * @param type A displayBarType representing the type of the display bar
     * @param remainingColor The color of the active section of the bar, that represents the remaining value, in the form of a glm::vec3.
     * @param lostColor The color of the inactive section of the bar, that represents the lost value, in the form of a glm::vec3.
     * @param backgroundColor The color of the outside section of the bar in the form of a glm::vec3.
     */
    DisplayBar(Shader* shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, GLfloat maxVal, std::string barText, DisplayBarType type, glm::vec3 remainingColor, glm::vec3 lostColor, glm::vec3 backgroundColor);
    
    //Destructor
    ~DisplayBar();
    
    //Public properties
    
    std::string text;
    
    //Public member functions
    
    /*!
     * A function to render the display bar. Should be called within the interface's rendering function.
     */
    void render();
    
    /*!
     * A function to set the value of the display bar. If the value is greater than the max value or less than 0, it is not changed.
     *
     * @param value A float representing the value to set, as long as it is within range.
     */
    void setValue(GLfloat value);
    
    /*!
     * A function to set the maximum value of the display bar. If the value is 0 or less, it is not changed.
     *
     * @param value A float representing the value to set, as long as it is within range.
     */
    void setMaxValue(GLfloat maxValue);
    
    //Public get functions
    
    /*!
     * @return The current value of the display bar.
     */
    GLfloat value();
    
    /*!
     * @return The current maximum value of the display bar.
     */
    GLfloat maxValue();
    
    /*!
     * @return The displayBartype of this display bar.
     */
    DisplayBarType type();
    
private:
    //Button properties
    GLfloat currentValue;
    GLfloat currentMaxValue;
    
    //OpenGL and GLFW properties
    Window* window;
    Shader barShader; //A compiled shader to render this display bar
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint locationVBO;
    GLuint filledVBO;
    
    glm::vec3 remainingValueColor;
    glm::vec3 lostValueColor;
    glm::vec3 outsideColor;
    
    DisplayBarType barType;
    
    Font font;
    
    //Viewport information
    GLfloat lowerLeftX;
    GLfloat lowerLeftY;
    GLfloat barWidth;
    GLfloat barHeight;
    
    GLfloat interfaceBoxLowerLeftX;
    GLfloat interfaceBoxLowerLeftY;
    GLfloat interfaceBoxWidth;
    GLfloat interfaceBoxHeight;
    
    //Private member functions
    
};

#endif /* DisplayBar_hpp */
