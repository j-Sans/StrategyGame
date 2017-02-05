//
//  Interface.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/9/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Interface_hpp
#define Interface_hpp

#include "Macros.h"

//Standard library includes
#include <list>
#include <map>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//Local includes
#include "Shader.hpp"
#include "Font.hpp"
#include "Window.hpp"
#include "Box.hpp"
#include "Button.hpp"
#include "DisplayBar.hpp"

//A unique type for use in storing the interfaces. They are stored in an std::map, with the keys being an interfaceType
enum interfaceType {
    default_left, //The default left interface, where game options are
    
    default_bottom, //The default bottom interface, where buying options are
    default_right, //The default right interface, initially empty
    
    creature, //The default creature interface
    building, //The default building interface
    
    settings, //The settings popup menu
    
    interface_other, //Anything else
};

class Interface {
public:
    //Constructors
    
    //Default constructor. Don't use this, it is only to allow interface objects to be declared without causing an error with memory.cpp
    Interface();
    
    /*!
     * A class representing an interface box.
     *
     * @param shader A pointer to a compiled shader for the interface box and other textured objects to use for rendering.
     * @param window A pointer to the current window object.
     * @param x The lower left corner's x coordinate, for setting the interface box. Should be in terms of GLFW screen coordinates, where (0,0) is the lower left corner of the screen.
     * @param y The lower left corner's y coordinate, for setting the interface box. Similar settings and configurations as with x.
     * @param width The width of the interface box, in GLFW screen coordinates.
     * @param height The height of the interface box, in GLFW screen coordinates.
     * @param texture The texture to use with the interface.
     * @param type What type of interface should be created, using an interfaceType enum. (See Interface.hpp)
     */
    Interface(Shader* shader, Shader* shaderForDisplayBars, Window* window, GLuint x, GLuint y, GLuint width, GLuint height, Texture texture, interfaceType type = interface_other
              );
    
    ~Interface();
    
    //Public properties
    
    /*!
     * An std::list of the boxes contained in this interface.
     */
    std::list<Box> boxes; //So that the boxes can be altered.
    
    /*!
     * An std::list of the buttons contained in this interface.
     */
    std::list<Button> buttons; //So that the buttons can be accessed by the game.
    
    /*!
     * An std::map of the boses contained in this interface, with key values of type displayBoxType (see Box.hpp)
     */
    std::map<DisplayBoxType, Box> boxMap; //So that the boxes can be accessed by the game. For specific types of boxes to be updated during the game.
    
    /*!
     * An std::map of the display bars contained in this interface, with key values of type displayBarType (see DisplayBar.hpp)
     */
    std::map<DisplayBarType, DisplayBar> displayBars; //So that the display bars can be accessed by the game.
    
    /*!
     * A boolean representing if this interface should be active or not. It won't render if this is false.
     */
    bool active = true;
    
    //Public member functions
    
    /*!
     * A function to render the interface. Should be called within the game loop's render function. This also renders the buttons on this interface.
     *
     * @param mouseDown A boolean representing if the mouse is down, for the buttons to figure out whether to become pressed.
     * @param mouseUp A boolean representing if the mouse has just been released. This is used for when the button resets its 'down' state.
     * @param buttonInteraction A boolean representing whether or not buttons should change color or jsut stay at default. This can be for when the settings menu is up, so button interaction outside of the menu doesn't occur.
     */
    void render(bool mouseDown, bool mouseUp, bool buttonInteraction);
    
    /*!
     * A function to add a button to the interface. This increments the layer that new properties will appear at.
     *
     * @param action An std::string that represents the action to occur when the button is pressed.
     * @param text An std::string that will be displayed on the button.
     */
    void addButton(std::string action, std::string text);
    
    /*!
     * A function to add a box to the interface. This increments the layer that new properties will appear at.
     *
     * @param text An std::string that will be displayed on the button.
     */
    void addBox(std::string text);
    
    /*!
     * A function to remove the previous layer of properties. Nothing will happen if there are no properties on the previous layer or if the previous layer contains properties with which the interface was constructed. That means that if the constructor explicitly adds properties, those will not be removed. If those are the next layer, false will be returned.
     *
     * @return True if a layer, or anything at all, was removed. False otherwise.
     */
    bool removePropertyLayer();
    
private:
    //OpenGL and GLFW properties
    Window* window;
    Shader *textureShader; //Compiled shader
    Shader *displayBarShader; //Shader for the display bars
    GLuint VAO; //VAO (Vertex Array Object) stores objects that can be drawn, including VBO data with the linked shader
    //VBO (Vertex Buffer Object) stores vertex data in the GPU graphics card. Will be stored in VAO
    GLuint VBO;
    
    Texture propertyTex;
    Texture interfaceTex;
    
    //Property information
    float nextPropertyHeight = 0.85; //The height at which the next button or box will be added.
    float initialPropertyHeight = 0.85;
    
    //Viewport information
    GLuint lowerLeftX;
    GLuint lowerLeftY;
    GLuint boxWidth;
    GLuint boxHeight;
    
//    int viewportWidth;
//    int viewportHeight;
    
    //Private member functions
    
    /*!
     * Update the viewport information of this object. This functino would be relevant with resizing, but for now do not use this function! The function works, but there is no substitute with the buttons!
     */
//    void updateViewport();
    
};

#endif /* Interface_hpp */
