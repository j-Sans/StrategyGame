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
    
    //So it starts off thinking all keys have just been pressed
    for (int a = 0; a < 1024; a++) {
        this->keysJustPressed[a] = false;
    }
    
    Texture tex;
    tex.set("Resources/background.jpg", 0, "tex");
    
    this->buttonTexture.set("Resources/button_tex.jpg", 30, "tex");
    
    this->interfaceShader = Shader("Shaders/interface/interface.vert", "Shaders/interface/interface.frag");
    this->textureShader = Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag");
    this->buttonShader = Shader("Shaders/button/button.vert", "Shaders/button/button.frag");
    this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
    
    this->background = TextureBox(this->textureShader, &this->window, 0, 0, 1, 1, 0, 0, 1, 1, "", tex);
    
    glm::ivec2 framebufferSize = this->window.framebufferSize();
    
    this->interface = Interface(&this->interfaceShader, &this->textureShader, &this->displayBarShader, &this->window, framebufferSize.x * 0.4, framebufferSize.y * 0.2, framebufferSize.x * 0.2, framebufferSize.y * 0.6, interface_other);
    
    this->interface.addButton("start", "Play", this->buttonTexture);
}

void Menu::render(ClientSocket* socketPtr) {
    glm::ivec2 framebufferSize = this->window.framebufferSize();
    this->window.setViewport(0, 0, framebufferSize.x, framebufferSize.y);
    
    this->background.render();
    this->interface.render(*this->mouseDown, *this->mouseUp, true);
    
    //Go through the buttons and check if they are pressed, and do any consequential actions
    for (auto button = this->interface.buttons.begin(); button != this->interface.buttons.end(); button++) {
        if (button->isPressed()) {
            this->processAction(button->action);
            break;
        }
    }
    
    if (this->textbox != nullptr && this->connecting == false) {
        this->updateTextbox("Input host name");
    }
    
    if (this->connected) {
        this->connecting = false;
        *socketPtr = this->socket;
    }
    
    if (this->failedToConnect) {
        this->connecting = false;
        this->interface.removePropertyLayer();
        this->interface.addButton("find_host", "Find host", this->buttonTexture);
        this->failedToConnect = false;
    }
    
    this->window.updateScreen();
}

bool Menu::getShouldWindowClose() {
    return this->window.shouldClose();
}

void Menu::processAction(std::string action) {
    if (action == "start") {
        this->interface.removePropertyLayer();
        this->interface.addBox("Input host name", this->buttonTexture);
        this->textbox = &this->interface.boxes.back();
        this->interface.addButton("find_host", "Find host", this->buttonTexture);
    } else if (action == "find_host") {
        if (this->textbox == nullptr) {
            throw std::logic_error("No host submitted: Textbox is nullptr.");
        }
        this->thread = std::thread(this->threadFuntion, &this->connected, &this->failedToConnect, &this->socket);
        this->connecting = true;
        this->interface.removePropertyLayer();
        this->interface.addBox("Looking for host", this->buttonTexture);
    }
}

void Menu::updateTextbox(std::string textboxDefaultStr) {
    std::string* text = &this->textbox->text;
    for (int key = GLFW_KEY_0; key < GLFW_KEY_9; key++) {
        if (this->keys[key] && !this->keysJustPressed[key]) {
            if (*text == "Input host name") {
                *text = "";
            }
            *text += key;
            this->keysJustPressed[key] = true;
        } else if (!this->keys[key]) {
            this->keysJustPressed[key] = false;
        }
    }
    for (int key = GLFW_KEY_A; key < GLFW_KEY_Z; key++) {
        if (this->keys[key] && !this->keysJustPressed[key]) {
            if (*text == "Input host name") {
                *text = "";
            }
            *text += key;
            this->keysJustPressed[key] = true;
        } else if (!this->keys[key]) {
            this->keysJustPressed[key] = false;
        }

    }
    if (this->keys[GLFW_KEY_BACKSPACE] && this->textbox->text.size() > 0 && !this->keysJustPressed[GLFW_KEY_BACKSPACE] & *text != "Input host name") {
        *text = text->substr(0, text->size() - 1);
        if (text->size() == 0) {
            *text = textboxDefaultStr;
        }
        this->keysJustPressed[GLFW_KEY_BACKSPACE] = true;
    } else if (!this->keys[GLFW_KEY_BACKSPACE]) {
        this->keysJustPressed[GLFW_KEY_BACKSPACE] = false;
    }

    
}

void Menu::threadFuntion(bool *done, bool *failed, ClientSocket *socket) {
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    while ((std::chrono::steady_clock::now() - start).count() > MAX_CONNECTION_TIME) {
        try {
            socket->setSocket("localhost", 3000);
        } catch (std::runtime_error) {
            continue; //Keep trying to connect, for 10 seconds
        }
        *done = true; //If it connected without throwing an error
    }
    *failed = true;
}
