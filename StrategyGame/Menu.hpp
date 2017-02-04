//
//  Menu.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include "Macros.h"

//Standard library includes
#include <thread>

//Local includes
#include "ClientSocket.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "TextureBox.hpp"
#include "Interface.hpp"

#define MAX_CONNECTION_TIME 5.0

class Menu {
public:
    //Constructor
    Menu(Window w, ClientSocket *sock, bool* mouseDown, bool* mouseUp, bool* keys);
    
    //Public properties
    
    /*!
     * The set width of the window. Due to high screen resolution, the final window may not actually be this many pixels wide.
     */
//    const GLuint windowWidth = 800;
    
    /*!
     * The set height of the window. Due to high screen resolution, the final window may not actually be this many pixels high.
     */
//    const GLuint windowHeight = 600;
    
    //Public member functions
    
    /*!
     * Renders the menu screen. Should be called every frame.
     */
    void render();
    
    /*!
     * @return A boolean indicating if the window should close.
     */
    bool getShouldWindowClose();
    
private:
    Window window;
    ClientSocket *socket;
    Interface interface;
    
    //Mouse info
    bool* mouseUp;
    bool* mouseDown;
    
    bool* keys;
    bool keysJustPressed[1024];
    Box* textbox = nullptr;
    
    //Thread
    std::thread thread;
    bool connecting = false;
    bool connected = false;
    bool failedToConnect = false;
    
    //Shaders
    Shader interfaceShader;
    Shader textureShader;
    Shader buttonShader;
    Shader displayBarShader;
    
    //Textures
    TextureBox background;
    Texture buttonTexture;
    
    //Private member functions
    
    /*!
     * A function to take std::strings representing actions to be done. The strings should be the actions from selected buttons.
     */
    void processAction(std::string action);
    
    /*!
     * A function to update the textbox. Should only be called if there is a text box.
     *
     * @param textboxDefaultStr The default string that should be displayed if there is no writing.
     */
    void updateTextbox(std::string textboxDefaultStr);
    
    /*!
     * A function to be run by the thread.
     */
    static void threadFuntion(bool* done, bool* failed, ClientSocket *socket);
};

#endif /* Menu_hpp */
