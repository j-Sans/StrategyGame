//
//  Button.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/10/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Button.hpp"

Button::Button(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat viewportWidth, GLfloat viewportHeight) {
    this->buttonWindow = window;
    this->buttonShader = shader;
    
    //Set viewport specifics
    this->lowerLeftX = x;
    this->lowerLeftY = y;
    this->boxWidth = width;
    this->boxHeight = height;
    
    //Put the box coordinates and information in terms of -1 to 1
    this->lowerLeftX *= 2.0;
    this->lowerLeftX -= 1.0;
    
    this->lowerLeftY *= 2.0;
    this->lowerLeftY -= 1.0;
    
    this->boxWidth *= 2.0;
    
    this->boxHeight *= 2.0;
    
    GLfloat data[] = {
        this->lowerLeftX, this->lowerLeftY,
        this->lowerLeftX + this->boxWidth, this->lowerLeftY,
        this->lowerLeftX, this->lowerLeftY + this->boxHeight,
        
        this->lowerLeftX + this->boxWidth, this->lowerLeftY,
        this->lowerLeftX, this->lowerLeftY + this->boxHeight,
        this->lowerLeftX + this->boxWidth, this->lowerLeftY + this->boxHeight,
    };
    
    std::cout << "(" << data[0] << ", " << data[1] << ")" << std::endl;
    std::cout << "(" << data[2] << ", " << data[3] << ")" << std::endl;
    std::cout << "(" << data[4] << ", " << data[5] << ")" << std::endl;
    std::cout << "(" << data[10] << ", " << data[11] << ")" << std::endl << std::endl;
    
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

void Button::render() {
    //Bind the VAO and draw shapes
    this->buttonShader->use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}