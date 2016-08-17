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

class DisplayBar {
public:
    //Constructors
    
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
     * @param text An std::string representing the text to display on the button as its name.
     * @param remainingColor The color of the active section of the bar, that represents the remaining value, in the form of a glm::vec3.
     * @param remainingColor The color of the inactive section of the bar, that represents the lost value, in the form of a glm::vec3.
     * @param remainingColor The color of the outside section of the bar in the form of a glm::vec3.
     */
    DisplayBar(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, GLfloat maxVal, std::string text, glm::vec3 remainingColor, glm::vec3 lostColor, glm::vec3 backgroundColor);
    
    //Public properties
    
    const GLfloat maxValue;
    
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
    
    //Public get functions
    
    /*!
     * @return The current value of the display bar.
     */
    GLfloat value();
    
    /*!
     * @return An std::string that is rendered on the bar.
     */
    std::string text();
    
private:
    //Button properties
    GLfloat currentValue;
    std::string barText; //A string to have as the title of the bar, what will be displayed.
    
    //OpenGL and GLFW properties
    GLFWwindow* barWindow;
    Shader* barShader; //Pointer to a compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint locationVBO;
    GLuint filledVBO;
    
    glm::vec3 remainingValueColor;
    glm::vec3 lostValueColor;
    glm::vec3 outsideColor;
    
    Font font;
    
    //Viewport information
    const GLfloat lowerLeftX;
    const GLfloat lowerLeftY;
    const GLfloat barWidth;
    const GLfloat barHeight;
    
    const GLfloat interfaceBoxLowerLeftX;
    const GLfloat interfaceBoxLowerLeftY;
    const GLfloat interfaceBoxWidth;
    const GLfloat interfaceBoxHeight;
    
    //Private member functions
    
};

#endif /* DisplayBar_hpp */
