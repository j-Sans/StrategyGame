//
//  Menu.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Menu.hpp"

Menu::Menu() {
    //Make a window object. May throw an error, which will be thrown through this function
    this->window.init(this->windowWidth, this->windowHeight, "Game", false, true);
    
    Texture tex;
    tex.set("Resources/background.jpg", 30, "tex");
    
    this->box = TextureBox(Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag"), &this->window, 0, 0, 1, 1, 0, 0, 1, 1, "", tex);
}

void Menu::render() {
    this->box.render();
    this->window.updateScreen();
}
