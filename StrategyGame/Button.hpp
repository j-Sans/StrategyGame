//
//  Button.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/10/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

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

class Button {
public:
    //Constructors
    
    /*!
     * A class representing a button on an interface block.
     *
     * @param shader A pointer to a compiled shader for rendering this button.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the button in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the button in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the button in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the button in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     */
    Button(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string action);
    
    //Public member functions
    
    /*!
     * A function to render the button. Should be called within the interface's rendering function.
     *
     * @param mouseDown A boolean representing if the mouse is currently pressed. This can set the button to being pressed.
     */
    void render(bool mouseDown);
    
    //Public get functions
    
    /*!
     * A function that returns the pressed state of the button as a boolean. If that value is true, then pressed is internally changed to false.
     *
     * @return Whether the button is pressed or not.
     */
    bool isPressed();
    
    /*!
     * @return An std::string representing the action that should be done when this button is pressed. The string should be interpreted in the Game class.
     */
    std::string action();
    
private:
    //Button properties
    bool pressed = false;
    std::string buttonAction; //A string to represent the actions that this button does.
    
    //OpenGL and GLFW properties
    GLFWwindow* buttonWindow;
    Shader* buttonShader; //Pointer to a compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint buttonVBO;
    GLuint colorVBO;
    
    //Viewport information
    const GLfloat lowerLeftX;
    const GLfloat lowerLeftY;
    const GLfloat buttonWidth;
    const GLfloat buttonHeight;
    
    const GLfloat interfaceBoxLowerLeftX;
    const GLfloat interfaceBoxLowerLeftY;
    const GLfloat interfaceBoxWidth;
    const GLfloat interfaceBoxHeight;
    
    //Private member functions
    
    /*!
     * A function to locate the mouse and update the color of the button if it should be highlighted.
     *
     * @param mouseDown A boolean representing if the mouse is currently pressed. This can set the button to being pressed.
     */
    void updateMouse(bool mouseDown);
    
};

#endif /* Button_hpp */
