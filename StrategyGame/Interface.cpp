//
//  Interface.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Interface.hpp"

//Only so that Visualizer.hpp can have properly initialize interfaces. No other purpose.
Interface::Interface() {}

Interface::Interface(Shader* shader, Shader* shaderForButtons, Shader* shaderForDisplayBars, GLFWwindow* window, GLuint x, GLuint y, GLuint width, GLuint height, interfaceType type) {
    this->interfaceWindow = window;
    this->interfaceShader = shader;
    this->buttonShader = shaderForButtons;
    this->displayBarShader = shaderForDisplayBars;
    
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
    
    switch (type) {
        case default_left:
            this->buttons.push_back(Button(this->buttonShader, this->interfaceWindow, 0.25f, 0.9f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "next turn", "End turn"));
            
            this->buttons.push_back(Button(this->buttonShader, this->interfaceWindow, 0.25f, 0.8f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "creature,Human,1,3,1,1,1,1,NORTH", "New Melee creature"));
            
            this->buttons.push_back(Button(this->buttonShader, this->interfaceWindow, 0.25f, 0.7f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "creature,Human,1,3,1,1,3,1,NORTH", "New 3-Ranged creature"));
            
            break;
        
        case creature:
            this->buttons.push_back(Button(this->buttonShader, this->interfaceWindow, 0.25f, 0.9f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "", "Test Button"));
            
            this->displayBars.push_back(DisplayBar(this->displayBarShader, this->interfaceWindow, 0.25f, 0.8f, 0.5f, 0.067f, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Health: 1/1", glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5)));
            
            break;
            
        case building:
            
            break;
            
        default: //For now includes the default bottom and default right interfaces
            
            break;
    }
    
    //Draw with OpenGL
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Interface::render(bool mouseDown, bool mouseUp) {
    if (active) {
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
            this->buttons[a].render(mouseDown, mouseUp);
        }
        
        for (GLuint a = 0; a < this->displayBars.size(); a++) {
            this->displayBars[a].render();
        }
        
        //Reset window information for game rendering
        glScissor(viewportWidth / 6.0, viewportHeight / 4.0, viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0);
        glViewport(viewportWidth / 6.0, viewportHeight / 4.0, viewportWidth * 2.0 / 3.0, viewportHeight * 3.0 / 4.0); //So that there is a 6th of the screen on both sides, and the bottom quarter of the screen left for interfaces
    }
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