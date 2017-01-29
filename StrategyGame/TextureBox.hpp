//
//  TextureBox.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#ifndef TextureBox_hpp
#define TextureBox_hpp

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

class TextureBox {
public:
    //Constructors
    
    //Default constructor. Don't use this, it is only to allow box objects to be declared without causing an error with memory.cpp
    TextureBox();
    
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
     * @param tex A texture object to be displayed on the box.
     */
    TextureBox(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string boxText, Texture tex);
    
    //Public properties
    
    std::string text; //A string to have as the title of the box, what will be displayed.
    
    //Public member functions
    
    /*!
     * A function to render the box. Should be called within the interface's rendering function.
     */
    void render();
    
private:
    //Box properties
    Texture texture;
    
    //OpenGL and GLFW properties
    Window* window;
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

#endif /* TextureBox_hpp */
