//
//  Interface.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Interface.hpp"

Interface::Interface(Shader* shader, Shader* shaderForButtons, GLFWwindow* window, GLuint x, GLuint y, GLuint width, GLuint height, bool withButtons) {
    this->interfaceWindow = window;
    this->interfaceShader = shader;
    this->buttonShader = shaderForButtons;
    
    //Set viewport specifics
    this->lowerLeftX = x;
    this->lowerLeftY = y;
    this->boxWidth = width;
    this->boxHeight = height;
    glfwGetFramebufferSize(this->interfaceWindow, &this->viewportWidth, &this->viewportHeight);
    
    GLfloat data[] = {
        -1.0, -1.0,
        -1.0,  1.0,
         1.0, -1.0,
        
        -1.0,  1.0,
         1.0, -1.0,
         1.0,  1.0,
    };
    
    if (withButtons) {
        this->buttons.push_back(Button(this->buttonShader, this->interfaceWindow, 0.25f, 0.9f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight));
    }
    
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
    //Get updated information about the viewport
    this->updateViewport();
    
    glViewport(this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight);
    
    //Set the box where OpenGL can draw
    glScissor(this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight);
    
    //Bind the VAO and draw shapes
    this->interfaceShader->use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    for (GLuint a = 0; a < this->buttons.size(); a++) {
        this->buttons[a].render();
    }
    
    //Reset window information for game rendering
    glScissor(viewportWidth / 6.0, viewportHeight / 4.0, viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0);
    glViewport(viewportWidth / 6.0, viewportHeight / 4.0, viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfaces
}

void Interface::updateViewport() {
    GLuint oldViewportWidth = this->viewportWidth;
    GLuint oldViewportHeight = this->viewportHeight;
    
    glfwGetFramebufferSize(this->interfaceWindow, &this->viewportWidth, &this->viewportHeight);
    
    //If the viewport size has changed, update the interface viewports by multiplying them by the ratio
    
    //Update the width
    if (oldViewportWidth != this->viewportWidth) {
        this->lowerLeftX *= (this->viewportWidth / oldViewportWidth);
        this->boxWidth *= (this->viewportWidth / oldViewportWidth);
    }
    
    //Update the height
    if (oldViewportHeight != this->viewportHeight) {
        this->lowerLeftY *= (this->viewportHeight / oldViewportHeight);
        this->boxHeight *= (this->viewportHeight / oldViewportHeight);
    }
}