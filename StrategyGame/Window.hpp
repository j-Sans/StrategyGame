//
//  Window.hpp
//  LibOpenGL
//
//  Created by Jake Sanders on 11/12/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include "Macros.h"

//Standard library includes
#include <string>
#include <iostream>
#include <exception>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW: Window functionality
#include <GLFW/glfw3.h>

//GLM: OpenGL math, including vectors
#include <glm/glm.hpp>

class Window {
public:
    //Constructor
    Window();
    
    //Public methods
    
    void init(int width, int height, std::string name, bool resizable, bool fillScreen);
    
    void setKeyCallback(GLFWkeyfun function);
    
    void setMouseButtonCallback(GLFWmousebuttonfun function);
    
    void setResizeCallback(GLFWwindowsizefun function);
    
    void setViewport(int x, int y, int width, int height);
    
    void setClearColor(float x, float y, float z);
    
    void setScissor(bool enable);
    
    void updateScreen();
    
    void close();
    
    void terminate();
    
    bool shouldClose();
    
    glm::ivec2 windowSize();
    
    glm::ivec2 framebufferSize();
    
    glm::dvec2 cursorPos();
    
    glm::vec2 viewportSize();
    
private:
    GLFWwindow* window;
    
    glm::vec2 viewportDimensions;
    
    glm::vec4 clearColor = glm::vec4(0.0, 0.0, 0.0, 1.0);
    
    bool set = false;
};

#endif /* Window_hpp */
