//
//  Font.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/27/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!In-Practice/Text-Rendering by Joey De Vries
//  This has been altered from the original code, and is not endoresed nor endorsed by LearnOpenGL
//

#ifndef Font_hpp
#define Font_hpp

#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

//GLM: OpenGL vectors
#include <GLM/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

struct Character {
    GLuint textureID; //ID handle of the glyph texture
    glm::ivec2 size; //Size of glyph
    glm::ivec2 bearing; //Offset from baseline to left/top of glyph
    GLuint advance; //Offset to advance to next glyph
};

class Font {
public:
    
    //Constructor
    Font(const char* fontPath);
    
    //Public properties
    
    //Public member functions
    
    //Public get functions
    
private:
    //Private properties
    
    //A map of chars to the corresponding character
    std::map<GLchar, Character> characters;
    
    //OpenGL buffers
    GLuint VAO;
    GLuint VBO;
    
    //Private member functions
};

#endif /* Font_hpp */
