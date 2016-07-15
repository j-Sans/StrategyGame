//
//  Button.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/10/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Button.hpp"

Button::Button(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight) : lowerLeftX((2.0 * x) - 1.0), lowerLeftY((2.0 * y) - 1.0), buttonWidth(2.0 * width), buttonHeight(2.0 * height), interfaceBoxlowerLeftX(interfaceX), interfaceBoxlowerLeftY(interfaceY), interfaceBoxWidth(interfaceWidth), interfaceBoxHeight(interfaceHeight) {

    //why do you multiply by 2.0 and then subtract 1.0? why are you making lowerLeftX negative?
    
    //This way they are from -1 to 1. Then i directly send the numbers from -1 to 1 to OpenGL, and OpenGL draws them like that. OpenGL thinks the left side of the interface box is -1 and the right side is 1.
    
    this->buttonWindow = window;
    this->buttonShader = shader;
    
    std::cout << "lowerLeftX: " << this->lowerLeftX << ", lowerLeftY: " << this->lowerLeftY << std::endl;
    std::cout << "buttonWidth: " << this->buttonWidth << ", buttonHeight: " << this->buttonHeight << std::endl;
    std::cout << "interfaceBoxLowerLeftX: " << this->interfaceBoxlowerLeftX << ", interfaceBoxLower: " << this->interfaceBoxlowerLeftY << std::endl;
    std::cout << "interfaceBoxWidth: " << this->interfaceBoxWidth << ", interfaceBoxHeight: " << this->interfaceBoxHeight << std::endl << std::endl;
    
    GLfloat data[] = {
        //Rectangle is drawn by two triangles
        this->lowerLeftX, this->lowerLeftY,
        this->lowerLeftX + this->buttonWidth, this->lowerLeftY,
        this->lowerLeftX, this->lowerLeftY + this->buttonHeight,
        
        this->lowerLeftX + this->buttonWidth, this->lowerLeftY,
        this->lowerLeftX, this->lowerLeftY + this->buttonHeight,
        this->lowerLeftX + this->buttonWidth, this->lowerLeftY + this->buttonHeight,
    };
    
    GLfloat color[6];
    for (int a = 0; a < 6; a++) {
        color[a] = 0.33f;
    }
    
    //We should have a constructor for circular buttons as well.
    
    //Draw with OpenGL
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->buttonVBO);
    glGenBuffers(1, &this->colorVBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Button::render() {
    this->updateMouse();
    
    //Bind the VAO and draw shapes
    this->buttonShader->use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Button::updateMouse() {
    glm::dvec2 mousePos;
    
    //Get the mouse position and set it to mousePos. It is relative to the upper left corner of the screen
    glfwGetCursorPos(this->buttonWindow, &mousePos.x, &mousePos.y);
    
    glm::ivec2 frameBufferSize;
    glfwGetFramebufferSize(this->buttonWindow, &frameBufferSize.x, &frameBufferSize.y);
    
    glm::ivec2 windowSize;
    glfwGetWindowSize(this->buttonWindow, &windowSize.x, &windowSize.y);
    
    mousePos.y = windowSize.y - mousePos.y;
    
    mousePos.x *= (double)frameBufferSize.x / (double)windowSize.x;
    mousePos.y *= (double)frameBufferSize.y / (double)windowSize.y;
    
    GLfloat actualButtonWidth = this->buttonWidth / 2.0; //From 0 to 1
    GLfloat actualButtonHeight = this->buttonHeight / 2.0; //From 0 to 1
    GLfloat actualButtonX = (1.0 + this->lowerLeftX) / 2.0; //From 0 to 1
    GLfloat actualButtonY = ((1.0 + this->lowerLeftY) / 2.0); //From 0 to 1. Reversed because mousePos is from upper left corner, not lower left.

    
    //Scale the dimensions with the interface box size
    actualButtonX *= this->interfaceBoxWidth;
    actualButtonY *= this->interfaceBoxHeight;
    actualButtonWidth *= this->interfaceBoxWidth;
    actualButtonHeight *= this->interfaceBoxHeight;
    
    actualButtonX += this->interfaceBoxlowerLeftX;
    actualButtonY += this->interfaceBoxlowerLeftY;
    
    std::cout << "actualX: " << actualButtonX << ", actualY: " << actualButtonY << std::endl;
    std::cout << "actualButtonWidth: " << actualButtonWidth << ", actualButtonHeight: " << actualButtonHeight << std::endl;
    std::cout << "interfaceBoxLowerLeftX: " << this->interfaceBoxlowerLeftX << ", interfaceBoxLower: " << this->interfaceBoxlowerLeftY << std::endl;
    std::cout << "interfaceBoxWidth: " << this->interfaceBoxWidth << ", interfaceBoxHeight: " << this->interfaceBoxHeight << std::endl << std::endl;
    
//    std::cout << "X: " << actualButtonX << ", Y: " << actualButtonY << std::endl;
    
    GLfloat color[6];
    
    //if (mousePos.x >= actualButtonX && mousePos.x <= actualButtonX + (actualButtonWidth * 3.0 / 4.0) && mousePos.y >= actualButtonY && mousePos.y <= actualButtonY + (actualButtonHeight * 3.0 / 4.0)) {
    
    if ((mousePos.x >= actualButtonX && mousePos.x <= actualButtonX + actualButtonWidth && mousePos.y >= actualButtonY && mousePos.y <= actualButtonY + actualButtonHeight)) {
        //First we bind the VAO
        glBindVertexArray(this->VAO);
        
        for (int a = 0; a < 6; a++) {
            color[a] = 0.67f;
        }
        
        //Color VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        
        //Next we tell OpenGL how to interpret the array
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else {
        //First we bind the VAO
        glBindVertexArray(this->VAO);
        
        for (int a = 0; a < 6; a++) {
            color[a] = 0.33f;
        }
        
        //Color VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        
        //Next we tell OpenGL how to interpret the array
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}