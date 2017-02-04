//
//  Menu.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Menu.hpp"

Menu::Menu(Window w, bool* mouseDown, bool* mouseUp, bool* keys) {
    //Make a window object. May throw an error, which will be thrown through this function
//    this->window.init(this->windowWidth, this->windowHeight, "Game", false, true);
    
    this->window = w;
    
    this->mouseDown = mouseDown;
    this->mouseUp = mouseUp;
    this->keys = keys;
    
    Texture tex;
    tex.set("Resources/background.jpg", 30, "tex");
    
    this->interfaceShader = Shader("Shaders/interface/interface.vert", "Shaders/interface/interface.frag");
    this->buttonShader = Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag");
    this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
    
    this->background = TextureBox(this->buttonShader, &this->window, 0, 0, 1, 1, 0, 0, 1, 1, "", tex);
    
    glm::ivec2 framebufferSize = this->window.framebufferSize();
    
    this->interface = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, &this->window, framebufferSize.x * 0.4, framebufferSize.y * 0.2, framebufferSize.x * 0.2, framebufferSize.y * 0.6, interface_other);
    
    this->interface.addButton("start", "Play");
}

void Menu::render() {
    glfwPollEvents();
    
    glm::ivec2 framebufferSize = this->window.framebufferSize();
    this->window.setViewport(0, 0, framebufferSize.x, framebufferSize.y);
    
    this->background.render();
    this->interface.render(*this->mouseDown, *this->mouseUp, true);
    
    //Go through the buttons and check if they are pressed, and do any consequential actions
    for (auto button = this->interface.buttons.begin(); button != this->interface.buttons.end(); button++) {
        if (button->isPressed()) {
            this->processAction(button->action);
        }
    }
    
    if (this->textbox != nullptr) {
        this->updateTextbox("Input host name");
    }
    
    this->window.updateScreen();
}

void Menu::processAction(std::string action) {
    if (action == "start") {
        this->interface.removePropertyLayer();
        this->interface.addBox("Input host name");
        this->textbox = &this->interface.boxes.back();
        this->interface.addButton("find_host", "Find host");
    } else if (action == "find_host") {
        if (this->textbox == nullptr) {
            throw std::logic_error("No host submitted: Textbox is nullptr.");
        }
        //Socket stuff
    }
}

void Menu::updateTextbox(std::string textboxDefaultStr) {
    std::string* text = &this->textbox->text;
    for (int key = GLFW_KEY_0; key < GLFW_KEY_9; key++) {
        if (keys[key]) {
            *text += keys[key];
        }
    }
    for (int key = GLFW_KEY_A; key < GLFW_KEY_Z; key++) {
        if (keys[key]) {
            *text += keys[key];
        }
    }
    if (keys[GLFW_KEY_BACKSPACE] && this->textbox->text.size() > 0) {
        *text = text->substr(0, text->size() - 1);
        if (text->size() == 0) {
            *text = textboxDefaultStr;
        }
    }
    
}
