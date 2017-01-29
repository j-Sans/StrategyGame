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
    
    this->box = TextureBox(Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag"), &this->window, 0, 0, 1, 1, <#GLuint interfaceX#>, <#GLuint interfaceY#>, <#GLfloat interfaceWidth#>, <#GLfloat interfaceHeight#>, <#std::string boxText#>, <#Texture tex#>)
}
