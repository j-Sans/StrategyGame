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
    
    this->box = TextureBox(this->buttonShader, &this->window, 0, 0, 1, 1, 0, 0, 1, 1, "", tex);
    
    glm::ivec2 screenSize = this->window.windowSize();
    
    this->interface = Interface(&this->interfaceShader, &this->buttonShader, &this->displayBarShader, &this->window, screenSize.x * 0.4, screenSize.y * 0.25, screenSize.x * 0.2, screenSize.y * 0.5, menu);
}

void Menu::render() {
    this->box.render();
    this->interface.render(*this->mouseDown, *this->mouseUp, true);
    this->window.updateScreen();
}
