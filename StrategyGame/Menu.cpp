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
    
    this->interface.addButton("run_client", "Play as client");
    
    this->interface.addButton("run_both", "Play as host");
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
//        if (!runningHost) {
            this->status = READY_TO_PLAY;
//        }
    }
    
    if (this->failedToConnect) {
        this->connecting = false;
        this->interface.removePropertyLayer();
        if (PLAY_AS_HOST) {
            this->interface.addButton("begin_game_as_both", "An error occurred. Try again");
        } else {
            this->interface.addButton("find_host", "Find host");
        }
        this->thread.join();
        this->failedToConnect = false;
    }
    
    if (this->status == PLAY_AS_HOST && this->numberOfConnectionsBox != nullptr) {
        if (!this->connecting) {
            this->numberOfConnectionsBox->text = "Number of players (including you): " + std::to_string(this->numberOfConnections);
        } else {
            this->numberOfConnectionsBox->text = "Setting up game. Number of players (including you): " + std::to_string(this->numberOfConnections);
        }
    }
    
    this->window->updateScreen();
}

bool Menu::getShouldWindowClose() {
    return this->window->shouldClose();
}

int Menu::getStatus() {
    if (this->status == ADD_PLAYER) {
        this->status = PLAY_AS_HOST;
        return ADD_PLAYER;
    } else {
        return this->status;
    }
}

void Menu::processAction(std::string action) {
    if (action == "run_client") { //Run as just a client
        this->interface.removePropertyLayer(); //Remove "Play as client"
        this->interface.removePropertyLayer(); //Remove "Play as host"
        this->interface.addBox("Input host name");
        this->textbox = &this->interface.boxes.back();
        this->interface.addButton("find_host", "Find host");
        
    } else if (action == "run_both") { //Run as both a host and a client
        this->runningHost = true;
        this->status = PLAY_AS_HOST;
        // main() will start a host thread, and then Menu::render() will connect to it
        
        this->interface.removePropertyLayer(); //Remove "Play as client"
        this->interface.removePropertyLayer(); //Remove "Play as host"
        
        //Add a box to indicate the number of players connected so far
        this->interface.addBox("Number of players (including you): " + std::to_string(this->numberOfConnections));
        this->numberOfConnectionsBox = &this->interface.boxes.back();
        
        this->interface.addButton("begin_game_as_both", "Begin");
        
    } else if (action == "find_host") { //Look for a host based on information from the textbox
        if (this->textbox == nullptr) {
            throw std::logic_error("No host submitted: Textbox is nullptr.");
        }
        this->connectToHost(this->textbox->text);
        this->interface.removePropertyLayer();
        this->interface.addBox("Looking for host");
        
    } else if (action == "begin_game_as_both") { //Start the game when playing as both a host and a client
        this->status = READY_TO_PLAY; //Because main() already connected the sockets
//        this->connectToHost("localhost");
        
    } else if (action.find("connect_to_host:") != std::string::npos) { //Connect to a host
        std::string hostName = action.substr(16); //The string after "connect_to_host:"
        this->thread = std::thread(this->threadFuntion, &this->connected, &this->failedToConnect, this->socket, hostName);
        this->connecting = true;
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
    if (this->keys[GLFW_KEY_APOSTROPHE] && !this->keysJustPressed[GLFW_KEY_APOSTROPHE]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] ? '\"' : '\'';
        this->keysJustPressed[GLFW_KEY_APOSTROPHE] = true;
    } else if (!this->keys[GLFW_KEY_APOSTROPHE]) {
        this->keysJustPressed[GLFW_KEY_APOSTROPHE] = false;
    }
    if (this->keys[GLFW_KEY_SPACE] && !this->keysJustPressed[GLFW_KEY_SPACE]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += ' ';
        this->keysJustPressed[GLFW_KEY_SPACE] = true;
    } else if (!this->keys[GLFW_KEY_SPACE]) {
        this->keysJustPressed[GLFW_KEY_SPACE] = false;
    }
    if (this->keys[GLFW_KEY_MINUS] && !this->keysJustPressed[GLFW_KEY_MINUS]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] ? '_' : '-';
        this->keysJustPressed[GLFW_KEY_MINUS] = true;
    } else if (!this->keys[GLFW_KEY_MINUS]) {
        this->keysJustPressed[GLFW_KEY_MINUS] = false;
    }
    if (this->keys[GLFW_KEY_PERIOD] && !this->keysJustPressed[GLFW_KEY_PERIOD]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] ? '>' : '.';
        this->keysJustPressed[GLFW_KEY_PERIOD] = true;
    } else if (!this->keys[GLFW_KEY_PERIOD]) {
        this->keysJustPressed[GLFW_KEY_PERIOD] = false;
    }
    if (this->keys[GLFW_KEY_COMMA] && !this->keysJustPressed[GLFW_KEY_COMMA]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] ? '<' : ',';
        this->keysJustPressed[GLFW_KEY_COMMA] = true;
    } else if (!this->keys[GLFW_KEY_COMMA]) {
        this->keysJustPressed[GLFW_KEY_COMMA] = false;
    }
    if (this->keys[GLFW_KEY_SEMICOLON] && !this->keysJustPressed[GLFW_KEY_SEMICOLON]) {
        if (*text == "Input host name") {
            *text = "";
        }
        *text += keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT] ? ':' : ';';
        this->keysJustPressed[GLFW_KEY_SEMICOLON] = true;
    } else if (!this->keys[GLFW_KEY_SEMICOLON]) {
        this->keysJustPressed[GLFW_KEY_SEMICOLON] = false;
    }
}

void Menu::connectToHost(std::string hostName) {
    this->processAction("connect_to_host:" + hostName);
}

void Menu::threadFuntion(bool *done, bool *failed, ClientSocket *socket, std::string hostName) {
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    while (true) {
        std::chrono::duration<double> timeElapsed = std::chrono::steady_clock::now() - start;
        if (timeElapsed.count() > MAX_CONNECTION_TIME) break;
        
        try {
            socket->setSocket(hostName.c_str(), 3000);
        } catch (std::runtime_error) {
            continue; //Keep trying to connect, for 10 seconds
        }
        *done = true; //If it connected without throwing an error, mark the connection was successful and end the function
        return;
    }
    *failed = true;
}
