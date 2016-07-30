//
//  Button.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/10/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Button.hpp"

//Public member functions

Button::Button(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string action, std::string text) : lowerLeftX((2.0 * x) - 1.0), lowerLeftY((2.0 * y) - 1.0), buttonWidth(2.0 * width), buttonHeight(2.0 * height), interfaceBoxLowerLeftX(interfaceX), interfaceBoxLowerLeftY(interfaceY), interfaceBoxWidth(interfaceWidth), interfaceBoxHeight(interfaceHeight), buttonAction(action), buttonText(text) {
    
    this->buttonWindow = window;
    this->buttonShader = shader;
    
    this->font = Font(FONT_PATH);
    
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
    
//    We should have a constructor for circular buttons as well.
    
//    We could, but that can be difficult. I'm not sure how to do them exactly, and while it would probably say on google, I don't think we need it yet
    
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

void Button::render(bool mouseDown){
    this->updateMouse(mouseDown);
    
    //Bind the VAO and draw shapes
    this->buttonShader->use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    std::cout << this->buttonText << std::endl;
    
    //Render the text on the button
    this->font.render(this->buttonText, (this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight, 1, glm::vec3(1.0f, 1.0f, 1.0f), this->interfaceBoxWidth, this->interfaceBoxHeight);
}

bool Button::isPressed() {
    if (this->pressed) { //If the button is pressed, reset pressed because it is being dealt with here
        this->pressed = false;
        return true;
    } else {
        return false;
    }
}

std::string Button::action() {
    return this->buttonAction;
}

//Private member functions

void Button::updateMouse(bool mouseDown) {
    glm::dvec2 mousePos;
    
    //Get the mouse position and set it to mousePos. It is relative to the upper left corner of the screen
    glfwGetCursorPos(this->buttonWindow, &mousePos.x, &mousePos.y);
    
    //Get the size of the window in terms of pixels, which is what the interface coordinates are in terms of
    glm::ivec2 frameBufferSize;
    glfwGetFramebufferSize(this->buttonWindow, &frameBufferSize.x, &frameBufferSize.y);
    
    //Get the size of the window in terms of screen coordinates, which is what the mouse coordinates are in terms of
    glm::ivec2 windowSize;
    glfwGetWindowSize(this->buttonWindow, &windowSize.x, &windowSize.y);
    
    //Flip the mouse's y so it is based from bottom left, not top left, corner
    mousePos.y = windowSize.y - mousePos.y;
    
    //Convert mouse to pixel coordinates from screen coordinates
    mousePos.x *= (double)frameBufferSize.x / (double)windowSize.x;
    mousePos.y *= (double)frameBufferSize.y / (double)windowSize.y;
    
    GLfloat actualButtonWidth = this->buttonWidth / 2.0; //From 0 to 1
    GLfloat actualButtonHeight = this->buttonHeight / 2.0; //From 0 to 1
    GLfloat actualButtonX = (1.0 + this->lowerLeftX) / 2.0; //From 0 to 1
    GLfloat actualButtonY = (1.0 + this->lowerLeftY) / 2.0; //From 0 to 1
    
    //Scale the dimensions with the interface box size
    actualButtonX *= this->interfaceBoxWidth;
    actualButtonY *= this->interfaceBoxHeight;
    actualButtonWidth *= this->interfaceBoxWidth;
    actualButtonHeight *= this->interfaceBoxHeight;
    
    actualButtonX += this->interfaceBoxLowerLeftX;
    actualButtonY += this->interfaceBoxLowerLeftY;
    
    GLfloat color[6];
    
    //Make the button darker if it is pressed.
    if (this->pressed) {
        //First we bind the VAO
        glBindVertexArray(this->VAO);
        
        for (int a = 0; a < 6; a++) {
            color[a] = 0.17f;
        }
        
        //Color VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        
        //Next we tell OpenGL how to interpret the array
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else if ((mousePos.x >= actualButtonX && mousePos.x <= actualButtonX + actualButtonWidth) && (mousePos.y >= actualButtonY && mousePos.y <= actualButtonY + actualButtonHeight)) { //Otherwise, highlight if it is being hovered
        
        //If the mouse is down at this button, make the button pressed
        if (mouseDown)
            this->pressed = true;
        
        //If the mouse is highlighting over the button
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