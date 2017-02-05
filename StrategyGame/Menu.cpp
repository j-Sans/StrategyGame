//
//  Menu.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#include "Menu.hpp"

Menu::Menu(Window* w, ClientSocket* sock, bool* mouseDown, bool* mouseUp, bool* keys) {
    //Make a window object. May throw an error, which will be thrown through this function
//    this->window.init(this->windowWidth, this->windowHeight, "Game", false, true);
    
    this->window = w;
    this->socket = sock;
    
    this->mouseDown = mouseDown;
    this->mouseUp = mouseUp;
    this->keys = keys;
    
    //So it starts off thinking all keys have just been pressed
    for (int a = 0; a < 1024; a++) {
        this->keysJustPressed[a] = false;
    }
    
    Texture backgroundTex;
    backgroundTex.set("Resources/IMG_1560.jpg", 0, "tex");
    Texture interfaceTex;
    interfaceTex.set("Resources/parchment.png", 29, "tex");
    
    this->textureShader = Shader("Shaders/texture/texture.vert", "Shaders/texture/texture.frag");
    this->displayBarShader = Shader("Shaders/displayBar/displayBar.vert", "Shaders/displayBar/displayBar.geom", "Shaders/displayBar/displayBar.frag");
    
    this->background = Box(this->textureShader, this->window, 0, 0, 1, 1, 0, 0, 1, 1, "", other, backgroundTex);
    
    glm::ivec2 framebufferSize = this->window->framebufferSize();
    
    this->interface.set(&this->textureShader, &this->displayBarShader, this->window, framebufferSize.x * 0.4, framebufferSize.y * 0.2, framebufferSize.x * 0.2, framebufferSize.y * 0.6, interfaceTex);
    
    this->interface.addButton("start", "Play");
}

void Menu::render() {
    glm::ivec2 framebufferSize = this->window->framebufferSize();
    this->window->setViewport(0, 0, framebufferSize.x, framebufferSize.y);
    
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
    }
    
    if (this->failedToConnect) {
        this->connecting = false;
        this->interface.removePropertyLayer();
        this->interface.addButton("find_host", "Find host");
        this->thread.join();
        this->failedToConnect = false;
    }
    
    this->window->updateScreen();
}

bool Menu::getShouldWindowClose() {
    return this->window->shouldClose();
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
        this->thread = std::thread(this->threadFuntion, &this->connected, &this->failedToConnect, this->socket, this->textbox->text);
        this->connecting = true;
        this->interface.removePropertyLayer();
        this->interface.addBox("Looking for host");
    }
}

void Menu::updateTextbox(std::string textboxDefaultStr) {
    std::string* text = &this->textbox->text;
    if (text->size() < 32) {
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
            bool upperCase = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] || keys[GLFW_KEY_CAPS_LOCK];
            if (this->keys[key] && !this->keysJustPressed[key]) {
                if (*text == "Input host name") {
                    *text = "";
                }
                *text += upperCase ? key : key + 32;
                this->keysJustPressed[key] = true;
            } else if (!this->keys[key]) {
                this->keysJustPressed[key] = false;
            }

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

void Menu::threadFuntion(bool *done, bool *failed, ClientSocket *socket, std::string hostName) {
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    while (true) {
        std::chrono::duration<double> timeElapsed = std::chrono::steady_clock::now() - start;
        if (timeElapsed.count() > MAX_CONNECTION_TIME) break;
        
        try {
            socket->setSocket(hostName, 3000);
        } catch (std::runtime_error) {
            continue; //Keep trying to connect, for 10 seconds
        }
        *done = true; //If it connected without throwing an error
        break;
    }
    *failed = true;
}
