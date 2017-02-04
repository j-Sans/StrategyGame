//
//  TextureBox.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "TextureBox.hpp"

//Public member functions

//Only so that Interface.hpp can have properly initialized boxes. No other purpose.
TextureBox::TextureBox() {}

TextureBox::TextureBox(Shader shader, Window* window, GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint interfaceX, GLuint interfaceY, GLfloat interfaceWidth, GLfloat interfaceHeight, std::string boxText, Texture tex) : lowerLeftX((2.0 * x) - 1.0), lowerLeftY((2.0 * y) - 1.0), boxWidth(2.0 * width), boxHeight(2.0 * height), interfaceBoxLowerLeftX(interfaceX), interfaceBoxLowerLeftY(interfaceY), interfaceBoxWidth(interfaceWidth), interfaceBoxHeight(interfaceHeight), text(boxText), texture(tex) {
    
    this->window = window;
    this->boxShader = shader;
    
    this->font = Font(FONT_PATH);
    
    GLfloat data[] = {
        //Rectangle is drawn by two triangles                                   //Texture   //Color data, signifying no modifiers on the texture hue
        this->lowerLeftX, this->lowerLeftY,                                     0.0, 0.0,   1.0, 1.0, 1.0,
        this->lowerLeftX + this->boxWidth, this->lowerLeftY,                    1.0, 0.0,   1.0, 1.0, 1.0,
        this->lowerLeftX, this->lowerLeftY + this->boxHeight,                   0.0, 1.0,   1.0, 1.0, 1.0,
        
        this->lowerLeftX + this->boxWidth, this->lowerLeftY,                    1.0, 0.0,   1.0, 1.0, 1.0,
        this->lowerLeftX, this->lowerLeftY + this->boxHeight,                   0.0, 1.0,   1.0, 1.0, 1.0,
        this->lowerLeftX + this->boxWidth, this->lowerLeftY + this->boxHeight,  1.0, 1.0,   1.0, 1.0, 1.0,
    };
    
    //Draw with OpenGL
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->boxVBO);
    
    //First we bind the VAO
    glBindVertexArray(this->VAO);
    
    //Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    //Next we tell OpenGL how to interpret the data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Next we tell OpenGL how to interpret the color
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //Next we tell OpenGL how to interpret the color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextureBox::render() {
    std::string boxText = ' ' + this->text + ' ';
    
    //Bind the VAO and draw shapes
    this->boxShader.use();
    
    this->texture.use(this->boxShader);
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    //Set the scale for the text
    GLfloat scale = 1.0f;
    
    //Get the text size
    glm::vec2 textSize = this->font.getSize(boxText, scale);
    
    //Initialized with the lower left corner position of the Box
    
    //The arithmatic done on the lower left corner converts it from -1 to 1 into 0 to 1
    glm::vec2 textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
    
    //The first .5 puts the Box width/height back to 0 to 1 (instead of -1 to 1)
    //The second .5 represents moving half of the Box width/height rather than the full width/height
    
    //Horizontally center the text so its center is at the Box's center
    textPos.x += ((this->boxWidth * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
    
    //Vertically center the text so its center is at the Box's center
    textPos.y += ((this->boxHeight * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    
    GLfloat sizeFraction = textSize.x / ((this->boxWidth * 0.5) * this->interfaceBoxWidth); //This represents what fraction of the Box the text will take up
    
    if (sizeFraction > 1) { //If the width of the text is greater than the Box size, scale down the text proportionally
        scale = 1.0 / sizeFraction;
        
        //Redo the size calculations with the new text size
        
        //Get the text size
        textSize = this->font.getSize(boxText, scale);
        
        textPos = glm::vec2((this->lowerLeftX + 1.0) / 2.0 * this->interfaceBoxWidth, (this->lowerLeftY + 1.0) / 2.0 * this->interfaceBoxHeight);
        
        //Horizontally center the text so its center is at the Box's center
        textPos.x += ((this->boxWidth * 0.5) * this->interfaceBoxWidth) * 0.5 - (textSize.x / 2.0);
        
        //Vertically center the text so its center is at the Box's center
        textPos.y += ((this->boxHeight * 0.5) * this->interfaceBoxHeight) * 0.5 - (textSize.y / 2.0);
    }
    
    //Render the text on the Box
    this->font.render(boxText, textPos.x, textPos.y, scale, glm::vec3(1.0f, 1.0f, 1.0f), this->interfaceBoxWidth, this->interfaceBoxHeight);
}
