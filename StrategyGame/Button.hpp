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
#include "Font.hpp"
#include "Texture.hpp"
#include "Window.hpp"

class Button {
public:
    //Constructors
    
    /*!
     * A class representing a button on an interface block.
     *
     * @param shader A compiled shader for rendering this button.
     * @param window A pointer to the current game window.
     * @param x A GLfloat representing the lower left x coordinate of the button in the current interface, from 0 to 1.
     * @param y A GLfloat representing the lower left y coordinate of the button in the current interface, from 0 to 1.
     * @param width A GLfloat representing the width of the button in the current interface, from 0 to 1.
     * @param height A GLfloat representing the height of the button in the current interface, from 0 to 1.
     * @param interfaceX A GLfloat representing the lower left x coordinate of the current interface, in screen coordinates.
     * @param interfaceY A GLfloat representing the lower left y coordinate of the current interface, in screen coordinates.
     * @param interfaceWidth A GLfloat representing the width of the current interface, in screen coordinates.
     * @param interfaceHeight A GLfloat representing the height of the current interface, in screen coordinates.
     * @param buttonAction An std::string representing the action which this button can do. This string is read in Visualizer::processButtons function. This string should only be something that can be read there. 
     * @param buttonText An std::string representing the text to display on the button as its name.
     * @param texture An optional texture parameter. If not added, then no texture will be added to the button.
     */
    Button(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string buttonAction, std::string buttonText, Texture texture = Texture());
    
    //Public properties
    
    constexpr const static GLfloat buttonDownTime = 0.25f;
    
    /*!
     * A string to represent the actions that this button does.
     */
    std::string action;
    
    /*!
     * A string to have as the title of the button, what will be displayed.
     */
    std::string text;
    
    //Public member functions
    
    /*!
     * A function to render the button. Should be called within the interface's rendering function.
     *
     * @param mouseDown A boolean representing if the mouse is currently pressed. This can set the button to being pressed.
     * @param mouseUp A boolean representing if the mouse has just been released. This is used for when the button resets its 'down' state.
     * @param butttonInteraction Whether the button should change color based on mouse position and clicks. This can be used for when settings menu is up.
     */
    void render(bool mouseDown, bool mouseUp, bool buttonInteraction);
    
    /*!
     * A function that returns the pressed state of the button as a boolean. If that value is true, then pressed is internally changed to false.
     *
     * @return Whether the button is pressed or not.
     */
    bool isPressed();
    
    /*!
     * @return Whether the button uses a texture.
     */
    bool hasTexture();
    
    /*!
     * @return The button height, from 0 to 1.
     */
    GLfloat getHeight();
    
    /*!
     * Frees OpenGL buffer data.
     */
    void terminate();
    
private:
    bool isSet = false;
    
    //Button properties
    bool pressed = false;
    bool hasBeenPressed = false; //Set to true after, keeps the button the darker 'pressed' color for longer
    
    //OpenGL and GLFW properties
    Window* window;
    Shader buttonShader; //A compiled shader to render this button
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint buttonVBO;
    GLuint colorVBO;
    
    Font font;
    
    Texture tex;
    
    //Viewport information
    GLfloat lowerLeftX;
    GLfloat lowerLeftY;
    GLfloat buttonWidth;
    GLfloat buttonHeight;
    
    GLfloat interfaceBoxLowerLeftX;
    GLfloat interfaceBoxLowerLeftY;
    GLfloat interfaceBoxWidth;
    GLfloat interfaceBoxHeight;
    
    //Private member functions
    
    /*!
     * A function to locate the mouse and update the color of the button if it should be highlighted.
     *
     * @param mouseDown A boolean representing if the mouse is currently pressed. This can set the button to being pressed.
     * @param mouseUp A boolean representing if the mouse has just been released. This is used for when the button resets its 'down' state.
     */
    void updateMouse(bool mouseDown, bool mouseUp);
    
};

#endif /* Button_hpp */
