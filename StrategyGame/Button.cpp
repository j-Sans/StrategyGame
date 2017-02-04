//
//  Button.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/10/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Button.hpp"

//Public member functions

Button::Button(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string buttonAction, std::string buttonText, Texture texture) : lowerLeftX((2.0 * x) - 1.0), lowerLeftY((2.0 * y) - 1.0), buttonWidth(2.0 * width), buttonHeight(2.0 * height), interfaceBoxLowerLeftX(interfaceX), interfaceBoxLowerLeftY(interfaceY), interfaceBoxWidth(interfaceWidth), interfaceBoxHeight(interfaceHeight), action(buttonAction), text(buttonText), tex(texture) {
    
    this->window = window;
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
    
    glm::vec3 color[6];
    
    if (this->tex.getSet()) {
        for (GLuint a = 0; a < 6; a++) {
            color[a] = glm::vec3(1.0, 1.0, 1.0);
        }
    } else {
        for (int a = 0; a < 6; a++) {
            color[a] = glm::vec3(0.33, 0.33, 0.33);
        }
    }
    
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Button::render(bool mouseDown, bool mouseUp, bool buttonInteraction) {
    if (buttonInteraction)
        this->updateMouse(mouseDown, mouseUp);
    
    std::string buttonText = ' ' + this->text + ' ';
    
    //Bind the VAO and draw shapes
    this->buttonShader.use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    //Set the scale for the text
    GLfloat scale = 1.0f;
    
    //Get the text size
    glm::vec2 textSize = this->font.getSize(buttonText, scale);
    
    //Initialized with the lower left corner position of the button
    
    //The arithmatic done on the lower left corner converts it from -1 to 1 into 0 to 1
    glm::vec2 textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
    
    //The first .5 puts the button width/height back to 0 to 1 (instead of -1 to 1)
    //The second .5 represents moving half of the button width/height rather than the full width/height
    
    //Horizontally center the text so its center is at the button's center
    textPos.x += ((this->buttonWidth * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
    
    //Vertically center the text so its center is at the button's center
    textPos.y += ((this->buttonHeight * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    
    GLfloat sizeFraction = textSize.x / ((this->buttonWidth * 0.5) * this->interfaceBoxWidth); //This represents what fraction of the button the text will take up
    
    if (sizeFraction > 1) { //If the width of the text is greater than the button size, scale down the text proportionally
        scale = 1.0 / sizeFraction;
        
        //Redo the size calculations with the new text size
        
        //Get the text size
        textSize = this->font.getSize(buttonText, scale);
        
        textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
        
        //Horizontally center the text so its center is at the button's center
        textPos.x += ((this->buttonWidth * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
        
        //Vertically center the text so its center is at the button's center
        textPos.y += ((this->buttonHeight * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    }
    
    //Render the text on the button
    this->font.render(buttonText, textPos.x, textPos.y, scale, glm::vec3(1.0f, 1.0f, 1.0f), this->interfaceBoxWidth, this->interfaceBoxHeight);
}

bool Button::isPressed() {
    if (this->pressed) {
        
        //If the button is pressed, reset pressed because it is being dealt with here
        this->pressed = false;
        
        //Only count it as pressed if the button is not dark from being recently pressed
        if (!this->hasBeenPressed) {
            this->hasBeenPressed = true;
            return true;
        }
        
    }
    return false;
}

//Private member functions

void Button::updateMouse(bool mouseDown, bool mouseUp) {
    glm::dvec2 mousePos = this->window->cursorPos();
    glm::ivec2 frameBufferSize = this->window->framebufferSize();
    glm::ivec2 windowSize = this->window->windowSize();
    
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
    
    glm::vec3 color[6];
    
    //Make the button darker if it is pressed.
    if (this->pressed) {
        //First we bind the VAO
        glBindVertexArray(this->VAO);
        
        if (this->tex.getSet()) {
            for (GLuint a = 0; a < 6; a++) {
                color[a] = glm::vec3(0.5, 0.5, 0.5);
            }
        } else {
            for (GLuint a = 0; a < 6; a++) {
                color[a] = glm::vec3(0.17, 0.17, 0.17);
            }
        }
        
        //Color VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        
        //Next we tell OpenGL how to interpret the array
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else {
        
        //If the mouse is hovering over the button
        if ((mousePos.x >= actualButtonX && mousePos.x <= actualButtonX + actualButtonWidth) && (mousePos.y >= actualButtonY && mousePos.y <= actualButtonY + actualButtonHeight)) {
        
            //If the mouse is down at this button, make the button pressed
            if (mouseDown) {
                this->pressed = true;
            }
            
            
            if (this->tex.getSet()) {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(1.5, 1.5, 1.5);
                }
            } else {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(0.67, 0.67, 0.67);
                }
            }
        } else {
            if (this->tex.getSet()) {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(1.0, 1.0, 1.0);
                }
            } else {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(0.33, 0.33, 0.33);
                }
            }
        }
        
        //Make the color darker if the button has been pressed recently. This has also been done if this->pressed is true
        if (this->hasBeenPressed) {
            if (this->tex.getSet()) {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(0.5, 0.5, 0.5);
                }
            } else {
                for (GLuint a = 0; a < 6; a++) {
                    color[a] = glm::vec3(0.17, 0.17, 0.17);
                }
            }
            
            if (mouseUp)
                this->hasBeenPressed = false;
        }
        
        //First we bind the VAO
        glBindVertexArray(this->VAO);
        
        //Color VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        
        //Next we tell OpenGL how to interpret the array
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
