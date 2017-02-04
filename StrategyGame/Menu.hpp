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

//Local includes
#include "ClientSocket.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "TextureBox.hpp"
#include "Interface.hpp"

class Menu {
public:
    //Constructor
    Menu(Window w, bool* mouseDown, bool* mouseUp, bool* keys);
    
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
    
private:
    Window window;
    ClientSocket socket;
    Interface interface;
    
    //Mouse info
    bool* mouseUp;
    bool* mouseDown;
    
    bool* keys;
    Box* textbox = nullptr;
    
    //Shaders
    Shader interfaceShader;
    Shader buttonShader;
    Shader displayBarShader;
    
    //Background image
    TextureBox background;
    
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
};

#endif /* Menu_hpp */
