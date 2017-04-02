//
//  Interface.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Interface.hpp"

void Interface::set(Shader* shader, Shader* shaderForDisplayBars, Window* window, GLuint x, GLuint y, GLuint width, GLuint height, Texture texture, interfaceType type) {
    this->isSet = true;
    this->window = window;
    this->textureShader = shader;
    this->displayBarShader = shaderForDisplayBars;
    
    this->propertyTex.set("Resources/button.jpg", 30, "tex");
    this->interfaceTex = texture;
    
    //Set viewport specifics
    this->lowerLeftX = x;
    this->lowerLeftY = y;
    this->boxWidth = width;
    this->boxHeight = height;
    
    GLfloat data[] = {
        //Vertices  //Texture   //Color
        -1.0, -1.0, 0.0, 0.0,   1.0, 1.0, 1.0,
        -1.0,  1.0, 0.0, 1.0,   1.0, 1.0, 1.0,
         1.0, -1.0, 1.0, 0.0,   1.0, 1.0, 1.0,
        
        -1.0,  1.0, 0.0, 1.0,   1.0, 1.0, 1.0,
         1.0, -1.0, 1.0, 0.0,   1.0, 1.0, 1.0,
         1.0,  1.0, 1.0, 1.0,   1.0, 1.0, 1.0,
    };
    
    this->initialPropertyHeight = this->nextPropertyHeight;
    
    switch (type) {
        case default_left: {
            
            this->buttons.push_back(Button(*this->textureShader, this->window, 0.25, this->nextPropertyHeight, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "return_to_menu", "Leave game", this->propertyTex));
            
            this->nextPropertyHeight -= 0.1;
            
            break;
        
        } case creature: {
            
            //Create the displayBar for health that will be added to the map
            DisplayBar healthBar(this->displayBarShader, this->window, 0.125, this->nextPropertyHeight, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Health: ", HealthBar, glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Inserts a display bar into the map with the key health
            this->displayBars.insert(std::pair<DisplayBarType, DisplayBar>(HealthBar, healthBar));
            
            this->nextPropertyHeight -= 0.1;
            
            //Create the displayBar for energy that will be added to the map
            DisplayBar energyBar(this->displayBarShader, this->window, 0.125, this->nextPropertyHeight, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Energy: ", EnergyBar, glm::vec3(0.2, 0.2, 0.4), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Insert the display bar into the map with the key energy
            this->displayBars.insert(std::pair<DisplayBarType, DisplayBar>(EnergyBar, energyBar));
            
            this->nextPropertyHeight -= 0.1;
            
            //Create the box for attack info that will be added to the map
            Box attackBox(*this->textureShader, this->window, 0.05, this->nextPropertyHeight, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Attack: ", creature_attack, this->propertyTex);
            
            //Insert the box into with the key creature_
            this->boxMap.insert(std::pair<DisplayBoxType, Box>(creature_attack, attackBox));
            
            //Create the box for range info that will be added to the map
            Box rangeBox(*this->textureShader, this->window, 0.525, this->nextPropertyHeight, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Range: ", creature_range, this->propertyTex);
            
            //Insert the box into with the key creature_range
            this->boxMap.insert(std::pair<DisplayBoxType, Box>(creature_range, rangeBox));
            
            this->nextPropertyHeight -= 0.1;
            
            //Create the box for vision info that will be added to the map
            Box visionBox(*this->textureShader, this->window, 0.05, this->nextPropertyHeight, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Vision: ", creature_vision, this->propertyTex);
            
            //Insert the box into with the key creature_vision
            this->boxMap.insert(std::pair<DisplayBoxType, Box>(creature_vision, visionBox));
            
            //Create the box for race info that will be added to the map
            Box raceBox(*this->textureShader, this->window, 0.525, this->nextPropertyHeight, 0.425, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, "Race: ", creature_race, this->propertyTex);
            
            //Insert the box into with the key creature_race
            this->boxMap.insert(std::pair<DisplayBoxType, Box>(creature_race, raceBox));
            
            this->nextPropertyHeight -= 0.1;
            
            break;
            
        } case building: {
            
            //Create the displayBar for health that will be added to the map
            DisplayBar healthBar(this->displayBarShader, this->window, 0.125, this->nextPropertyHeight, 0.75, 0.05, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, 0, "Health: ", HealthBar, glm::vec3(0.2, 0.4, 0.2), glm::vec3(0.67, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5));
            
            //Inserts a display bar into the map with the key health
            this->displayBars.insert(std::pair<DisplayBarType, DisplayBar>(HealthBar, healthBar));
            
            this->nextPropertyHeight -= 0.1;
            
            break;
            
        } default: //For now includes the default_right and settings interfaces
            
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //Next we tell OpenGL how to interpret the array
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Interface::render(bool mouseDown, bool mouseUp, bool buttonInteraction) {
    if (!this->isSet)
        throw std::logic_error("Interface not set");
    
    if (active) {
        //Get updated information about the viewport
//        this->updateViewport();
        
        this->window->setViewport(this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight);
        
        //Set the box where OpenGL can draw
//        glScissor(this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight);
        
        //Bind the VAO and draw shapes
        this->textureShader->use();
        
        this->interfaceTex.use(*this->textureShader);
        
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        for (auto box = this->boxes.begin(); box != this->boxes.end(); box++) {
            box->render();
        }
        
        for (auto box = this->boxMap.begin(); box != this->boxMap.end(); box++) {
            box->second.render();
        }
        
        for (auto button = this->buttons.begin(); button != this->buttons.end(); button++) {
            button->render(mouseDown, mouseUp, buttonInteraction);
        }
        
        for (auto displayBar = this->displayBars.begin(); displayBar != this->displayBars.end(); displayBar++) {
            displayBar->second.render();
        }
    }
}

void Interface::addButton(std::string action, std::string text) {
    if (!this->isSet)
        throw std::logic_error("Interface not set");
    
    this->buttons.push_back(Button(*this->textureShader, this->window, 0.25, this->nextPropertyHeight, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, action, text, this->propertyTex));
    
    this->nextPropertyHeight -= 0.1;
}

void Interface::addBox(std::string text) {
    if (!this->isSet)
        throw std::logic_error("Interface not set");
    
    this->boxes.push_back(Box(*this->textureShader, this->window, 0.25, this->nextPropertyHeight, 0.5, 0.067, this->lowerLeftX, this->lowerLeftY, this->boxWidth, this->boxHeight, text, other, this->propertyTex));
    
    this->nextPropertyHeight -= 0.1;
}

bool Interface::removePropertyLayer() {
    if (!this->isSet)
        throw std::logic_error("Interface not set");
    
    this->nextPropertyHeight += 0.1;
    if (this->nextPropertyHeight > this->initialPropertyHeight) {
        this->nextPropertyHeight -= 0.1;
        return false;
    }
    
    bool anythingRemoved = false;
    
    for (auto button = this->buttons.begin(); button != this->buttons.end(); button++) {
        if (button->getHeight() <= this->nextPropertyHeight) {
            button = this->buttons.erase(button);
            anythingRemoved = true;
        }
    }
    
    for (auto box = this->boxes.begin(); box != this->boxes.end(); box++) {
        if (box->getHeight() <= this->nextPropertyHeight) {
            box = this->boxes.erase(box);
            anythingRemoved = true;
        }
    }
    
    if (!anythingRemoved) this->nextPropertyHeight -= 0.1;
    
    return anythingRemoved;
}

Interface::~Interface() {
    if (this->isSet) {
        try {
            glDeleteVertexArrays(1, &this->VAO);
        } catch(...) {
            std::cout << "~Interface(): Unable to properly delete VAO. Error thrown with call of glDeleteVertexArrays(1, &this->VAO)." << std::endl;
        }
        try {
            glDeleteBuffers(1, &this->VBO);
        } catch(...) {
            std::cout << "~Interface(): Unable to properly delete VBO. Error thrown with call of glDeleteBuffers(1, &this->VBO)." << std::endl;
        }
        for (auto button = this->buttons.begin(); button != this->buttons.end(); button++) {
            try {
                button->terminate();
            } catch(...) {
                std::cout << "~Interface(): Unable to properly delete button from std::list<box>: " << &button << ". Error thrown with call of button->terminate()." << std::endl;
            }
        }
        for (auto box = this->boxes.begin(); box != this->boxes.end(); box++) {
            try {
                box->terminate();
            } catch(...) {
                std::cout << "~Interface(): Unable to properly delete box from std::list<box>: " << &box << ". Error thrown with call of box->terminate()." << std::endl;
            }
        }
        for (auto bar = this->displayBars.begin(); bar != this->displayBars.end(); bar++) {
            try {
                bar->second.terminate();
            } catch(...) {
                std::cout << "~Interface(): Unable to properly delete displayBar from std::map<displayBarType, displayBar>: " << &bar << ". Error thrown with call of bar->terminate()." << std::endl;
            }
        }
        for (auto box = this->boxMap.begin(); box != this->boxMap.end(); box++) {
            try {
                box->second.terminate();
            } catch(...) {
                std::cout << "~Interface(): Unable to properly delete box from std::map<boxType, Box>: " << &box << ". Error thrown with call of box->second.terminate()." << std::endl;
            }
        }
    }
}

//void Interface::updateViewport() {
//    GLuint oldViewportWidth = this->viewportWidth;
//    GLuint oldViewportHeight = this->viewportHeight;
//    
//    glfwGetFramebufferSize(this->window, &this->viewportWidth, &this->viewportHeight);
//    
//    //If the viewport size has changed, update the interface viewports by multiplying them by the ratio
//    
//    //Update the width
//    if (oldViewportWidth != this->viewportWidth) {
//        this->lowerLeftX *= (this->viewportWidth / oldViewportWidth);
//        this->boxWidth *= (this->viewportWidth / oldViewportWidth);
//    }
//    
//    //Update the height
//    if (oldViewportHeight != this->viewportHeight) {
//        this->lowerLeftY *= (this->viewportHeight / oldViewportHeight);
//        this->boxHeight *= (this->viewportHeight / oldViewportHeight);
//    }
//}
