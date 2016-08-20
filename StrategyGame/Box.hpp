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

class Box {
public:
    //Constructors
    
    /*!
     * A class representing a box on an interface block.
     *
     * @param shader A pointer to a compiled shader for rendering this box.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the box in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the box in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the box in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the box in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     * @param text An std::string representing the text to display on the box as its name.
     */
    Box(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string text);
    
    //Public properties
    
    //Public member functions
    
    /*!
     * A function to render the box. Should be called within the interface's rendering function.
     */
    void render();
    
    //Public get functions
    
    /*!
     * A function that returns the pressed state of the box as a boolean. If that value is true, then pressed is internally changed to false.
     *
     * @return Whether the box is pressed or not.
     */
    bool isPressed();
    
private:
    //Box properties
    std::string boxText; //A string to have as the title of the box, what will be displayed.
    
    //OpenGL and GLFW properties
    GLFWwindow* boxWindow;
    Shader boxShader; //A compiled shader to render this box
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint boxVBO;
    GLuint colorVBO;
    
    Font font;
    
    //Viewport information
    const GLfloat lowerLeftX;
    const GLfloat lowerLeftY;
    const GLfloat boxWidth;
    const GLfloat boxHeight;
    
    const GLfloat interfaceBoxLowerLeftX;
    const GLfloat interfaceBoxLowerLeftY;
    const GLfloat interfaceBoxWidth;
    const GLfloat interfaceBoxHeight;
    
    //Private member functions
    
};

#endif /* Box_hpp */
