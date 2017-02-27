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
#include "Interface.hpp"

#define MAX_CONNECTION_TIME 5.0

#define WAITING_FOR_INPUT 0
#define READY_TO_PLAY 1
#define PLAY_AS_HOST 2
#define HOST_GAME 3
#define ADD_PLAYER

class Menu {
public:
    //Constructor
    Menu(Window* w, ClientSocket* sock, bool* mouseDown, bool* mouseUp, bool* keys);
    
    //Public properties
    
    /*!
     * An int holding the number of clients connected. It is only used when the client is also hosting. It is updated by main().
     */
    unsigned int numberOfConnections = 0;
    
    //Public member functions
    
    /*!
     * Renders the menu screen. Should be called every frame.
     */
    void render();
    
    /*!
     * @return A boolean indicating if the window should close.
     */
    bool getShouldWindowClose();
    
    /*!
     * Gets the status. Also ... ***ADD_PLAYER SHOULD BE CHANGED WHEN READ***
     *
     * @return An int macro representing the status of the menu. Used to indicate to main() when to move on.
     */
    int getStatus();
    
private:
    Window* window;
    ClientSocket *socket;
    Interface interface;
    
    int status = WAITING_FOR_INPUT;
    
    bool runningHost = false;
    
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
    Shader textureShader;
    Shader displayBarShader;
    
    //Textures
    Box background;
    
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
     * A function to connect to a host with a given name.
     *
     * @param hostName The name of the host, as a std::string.
     */
    void connectToHost(std::string hostName);
    
    /*!
     * A function to be run by an std::thread that will attempt to connect to a host.
     *
     * @param done A pointer to a bool that will indicate when the function is done and successfully connected with a host. It can then be joined with the main thread.
     * @param failed A pointer to a bool indicating that no host was found.
     * @param socket A pointer to a ClientSocket object with the socket that should be connected to the host.
     * @param hostName A std::string representing the name of the host to connect to. Use "localhost" to connect to a host on the same device.
     */
    static void threadFuntion(bool* done, bool* failed, ClientSocket *socket, std::string hostName);
};

#endif /* Menu_hpp */
