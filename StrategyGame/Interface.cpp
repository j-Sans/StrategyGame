//
//  Interface.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Interface.hpp"

//Only so that Visualizer.hpp can have properly initialized interfaces. No other purpose.
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
        case default_left: {
            this->buttons.push_back(Button(*this->buttonShader, this->interfaceWindow, 0.25, 0.9, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "settings", "Settings"));
            
            this->buttons.push_back(Button(*this->buttonShader, this->interfaceWindow, 0.25, 0.8, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "next turn", "End turn"));
            
            this->buttons.push_back(Button(*this->buttonShader, this->interfaceWindow, 0.25, 0.7, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "creature,Human,1,3,1,1,1,1,NORTH", "New Melee creature"));
            
            this->buttons.push_back(Button(*this->buttonShader, this->interfaceWindow, 0.25, 0.6, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "creature,Human,1,3,1,1,3,1,NORTH", "New 3-Ranged creature"));
            
            break;
        
        } case creature: {
            
            //Create the box for attack info that will be added to the map
            Box attackBox(*this->buttonShader, this->interfaceWindow, 0.05, 0.7, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Attack: ", creature_attack);
            
            //Insert the box into with the key creature_
            this->boxes.insert(std::pair<displayBoxType, Box>(creature_attack, attackBox));
            
            //Create the box for range info that will be added to the map
            Box rangeBox(*this->buttonShader, this->interfaceWindow, 0.525, 0.7, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Range: ", creature_range);
            
            //Insert the box into with the key creature_range
            this->boxes.insert(std::pair<displayBoxType, Box>(creature_range, rangeBox));
            
            
            //Create the box for vision info that will be added to the map
            Box visionBox(*this->buttonShader, this->interfaceWindow, 0.05, 0.6, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Vision: ", creature_vision);
            
            //Insert the box into with the key creature_vision
            this->boxes.insert(std::pair<displayBoxType, Box>(creature_vision, visionBox));
            
            //Create the box for race info that will be added to the map
            Box raceBox(*this->buttonShader, this->interfaceWindow, 0.525, 0.6, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Race: ", creature_race);
            
            //Insert the box into with the key creature_race
            this->boxes.insert(std::pair<displayBoxType, Box>(creature_race, raceBox));
            
            
            //Create the displayBar for health that will be added to the map
            DisplayBar healthBar(this->displayBarShader, this->interfaceWindow, 0.125, 0.9, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Health: ", health_bar, glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Inserts a display bar into the map with the key health
            this->displayBars.insert(std::pair<displayBarType, DisplayBar>(health_bar, healthBar));
            
            
            //Create the displayBar for health that will be added to the map
            DisplayBar energyBar(this->displayBarShader, this->interfaceWindow, 0.125, 0.8, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Energy: ", energy_bar, glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Insert the display bar into the map with the key health
            this->displayBars.insert(std::pair<displayBarType, DisplayBar>(energy_bar, energyBar));
            
            break;
            
        } case building: {
            
            //Create the displayBar for health that will be added to the map
            DisplayBar healthBar(this->displayBarShader, this->interfaceWindow, 0.125, 0.9, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Health: ", health_bar, glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Inserts a display bar into the map with the key health
            this->displayBars.insert(std::pair<displayBarType, DisplayBar>(health_bar, healthBar));
            
            break;
            
        } case settings: {
            
            break;
        
        } default: //For now includes the default bottom and default right interfaces
            
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
        
        for (auto box = this->boxes.begin(); box != this->boxes.end(); box++) {
            box->second.render();
        }
        
        for (auto button = this->buttons.begin(); button != this->buttons.end(); button++) {
            button->render(mouseDown, mouseUp);
        }
        
        for (auto displayBar = this->displayBars.begin(); displayBar != this->displayBars.end(); displayBar++) {
            displayBar->second.render();
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