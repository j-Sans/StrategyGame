//
//  Interface.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Interface_hpp
#define Interface_hpp

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//Local includes
#include "Shader.hpp"

class Interface {
public:
    //Constructors
    Interface();
    
    Interface(const GLchar* vertexPath, const GLchar* fragmentPath, GLFWwindow* window, GLuint x, GLuint y, GLuint width, GLuint height);
    
    //Public member functions
    void render();
    
private:
    //OpenGL and GLFW properties
    GLFWwindow* interfaceWindow;
    Shader interfaceShader; //Compiled shader
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint VBO;
    
    //Viewport information
    GLuint lowerLeftX;
    GLuint lowerLeftY;
    GLuint boxWidth;
    GLuint boxHeight;
    
    int viewportWidth;
    int viewportHeight;
    
    //Private member functions
    void updateViewport();
    
};

#endif /* Interface_hpp */
