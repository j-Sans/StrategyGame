//
//  DisplayBar.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 8/16/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "DisplayBar.hpp"

DisplayBar::DisplayBar(Shader* shader, GLFWwindow* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, GLfloat maxVal, std::string text, glm::vec3 remainingColor, glm::vec3 lostColor, glm::vec3 backgroundColor) : lowerLeftX((2.0 * x) - 1.0), lowerLeftY((2.0 * y) - 1.0), barWidth(2.0 * width), barHeight(2.0 * height), interfaceBoxLowerLeftX(interfaceX), interfaceBoxLowerLeftY(interfaceY), interfaceBoxWidth(interfaceWidth), interfaceBoxHeight(interfaceHeight), currentMaxValue(maxVal), barText(text), remainingValueColor(remainingColor), lostValueColor(lostColor), outsideColor(backgroundColor) {
    
    this->currentValue = this->currentMaxValue;
    
    this->barWindow = window;
    this->barShader = *shader;
    
    this->font = Font(FONT_PATH);
    
    //We only need to send the center to the shader in the VBO. The shader figures out the rest using width and height uniforms
    
    GLfloat xCenter = this->lowerLeftX + width;
    GLfloat yCenter = this->lowerLeftY + height;
    
    //Create the rectangle data
    GLfloat data[] = {
        xCenter, yCenter,
    };

    GLfloat portionFilled[] = { this->currentValue / (GLfloat)this->currentMaxValue };
    
    //Initiate the OpenGL buffers
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->locationVBO);
    glGenBuffers(1, &this->filledVBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Location VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->locationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Filled portion VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->filledVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(portionFilled), portionFilled, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //Send the colors as uniforms to the shader
    
    this->barShader.use();
    
    this->barShader.uniform1f("width", this->barWidth);
    this->barShader.uniform1f("height", this->barHeight);
    
    this->barShader.uniform3f("remainingColor", this->remainingValueColor);
    this->barShader.uniform3f("lostColor", this->lostValueColor);
    this->barShader.uniform3f("outsideColor", this->outsideColor);
}

void DisplayBar::render() {
    std::string text = ' ' + this->barText + ' ';
    
    //Update the portionFilled data in the shader in case it been changed
    GLfloat portionFilled[] = { this->currentValue / (GLfloat)this->currentMaxValue };
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Filled portion VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->filledVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(portionFilled), portionFilled, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //Bind the VAO and draw shapes
    this->barShader.use();
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
    
    //The inset of the inner bar from the outer bar
    GLfloat widthInset = (GLfloat)this->barWidth / 10.0;
    GLfloat heightInset = (GLfloat)this->barHeight / 10.0;
    
    //Width and height variables representing the width or height of the inner bar, so text is sized for the inner bar rather than the outer one
    GLfloat width = this->barWidth - (2.0 * widthInset);
    GLfloat height = this->barHeight - (2.0 * heightInset);
    
    //Set the scale for the text
    GLfloat scale = 1.0f;
    
    //Get the text size
    glm::vec2 textSize = this->font.getSize(text, scale);
    
    //Initialized with the lower left corner position of the button
    
    //The arithmatic done on the lower left corner converts it from -1 to 1 into 0 to 1
    glm::vec2 textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
    
    //The first .5 puts the button width/height back to 0 to 1 (instead of -1 to 1)
    //The second .5 represents moving half of the button width/height rather than the full width/height
    
    //Horizontally center the text so its center is at the button's center
    textPos.x += ((width * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
    
    //Vertically center the text so its center is at the button's center
    textPos.y += ((height * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    
    GLfloat sizeFraction = textSize.x / ((width * 0.5) * this->interfaceBoxWidth); //This represents what fraction of the button the text will take up
    
    if (sizeFraction > 1) { //If the width of the text is greater than the button size, scale down the text proportionally
        scale = 1.0 / sizeFraction;
        
        //Redo the size calculations with the new text size
        
        //Get the text size
        textSize = this->font.getSize(text, scale);
        
        textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
        
        //Horizontally center the text so its center is at the button's center
        textPos.x += ((width * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
        
        //Vertically center the text so its center is at the button's center
        textPos.y += ((height * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    }
    
    //Translate the text so it is correctly placed within the smaller inner bar
    textPos.x += widthInset * this->interfaceBoxWidth / 2.0;
    textPos.y += heightInset * this->interfaceBoxHeight / 2.0;
    
    //Render the text on the button
    this->font.render(text, textPos.x, textPos.y, scale, glm::vec3(1.0f, 1.0f, 1.0f), this->interfaceBoxWidth, this->interfaceBoxHeight);
}

void DisplayBar::setValue(GLfloat value) {
    if (value >= 0 && value <= this->currentMaxValue) { //Check to make sure the inputted value is valid, being at most the max and at least 0.
        this->currentValue = value;
    }
}

void DisplayBar::setMaxValue(GLfloat maxValue) {
    if (maxValue >= 0) {
        this->currentMaxValue = maxValue;
    }
}

GLfloat DisplayBar::value() {
    return this->currentValue;
}

GLfloat DisplayBar::maxValue() {
    return this->currentMaxValue;
}

std::string DisplayBar::text() {
    return this->barText;
}