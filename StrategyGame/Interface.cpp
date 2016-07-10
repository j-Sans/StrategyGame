//
//  Interface.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Interface.hpp"

Interface::Interface() {}

Interface::Interface(const GLchar* vertexPath, const GLchar* fragmentPath, GLFWwindow* window) {
    this->interfaceWindow = window;
    this->interfaceShader = Shader(vertexPath, fragmentPath);
    
    GLfloat data[] = {
        -0.5, -0.5,
        -0.5,  0.5,
         0.5, -0.5,
        
        -0.5,  0.5,
         0.5, -0.5,
         0.5,  0.5,
    };
    
    //Draw with OpenGL
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    //Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Interface::render() {
    //Tell OpenGL window information
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(this->interfaceWindow, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth / 6, viewportHeight); //So that this is drawn on the first sixth of the screen
    
    
    //Bind the VAO and draw shapes
    this->interfaceShader.use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    //Reset window information for game rendering
    glViewport(viewportWidth / 6, viewportHeight / 4, viewportWidth * 2 / 3, viewportHeight * 3 / 4); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfacecs
}