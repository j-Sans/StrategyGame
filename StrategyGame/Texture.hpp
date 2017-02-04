//
//  Texture.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

//Standard library includes
#include <string>

//GLEW: Locates memory location of OpenGL functions
#define GLEW_STATIC
#include <GL/glew.h>

//SOIL: Sets up texture loading
#include <SOIL/SOIL.h>

//Local includes
#include "Shader.hpp"

class Texture {
public:
    
    /*!
     * A function to set the loaded texture that can directly be called to be used. The texture cannot be used if set has not been called.
     *
     * @param imagePath The path and filename of the image to be loaded in as a texture.
     * @param texNumber The texture number that the texture should be connected with. This can only be a number from 0 to 30. This is used, for example, when calling and activating it as GL_TEXTURE0, or whichever number. The 32nd texture, when texNumber = 31, is reserved for use by fonts.
     * @param uniformName The name of the sampler2D within GLSL to send the texture to when activated.
     */
    void set(const GLchar* imagePath, GLuint texNumber, const GLchar* uniformName);
    
    //Public member functions
    
    /*!
     * Activate the texture and send the information to the given shader. This should be called right after a shader was used.
     *
     * @param shader The shader that was is currently being used when the texture is activated. The texture will be sent to this shader.
     */
    const void use(Shader shader);
    
    /*!
     * @return If this texture is set.
     */
    bool getSet();
    
private:
    //Private properties
    GLuint tex;
    GLuint texNum;
    GLuint id;
    std::string name;
    
    bool isSet = false;
};

#endif /* Texture_hpp */
