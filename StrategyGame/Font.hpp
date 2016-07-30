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

//Standard library includes
#include <map>
#include <string>
#include <iostream>

//GLEW: A function that gets OpenGL functionality
#define GLEW_STATIC
#include <GL/glew.h>

//GLM: OpenGL vectors and matrix transformations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

//FreeType font library
#include <ft2build.h>
#include FT_FREETYPE_H

//Local includes
#include "Shader.hpp"

struct Character {
    GLuint textureID; //ID handle of the glyph texture
    glm::ivec2 size; //Size of glyph
    glm::ivec2 bearing; //Offset from baseline to left/top of glyph
    GLuint advance; //Offset to advance to next glyph
};

class Font {
public:
    
    //Constructors
    Font();
    
    Font(const char* fontPath);
    
    //Public properties
    
    //Public member functions
    void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, GLfloat windowWidth, GLfloat windowHeight);
    
    //Public get functions
    
private:
    //Private properties
    
    //A map of chars to the corresponding character
    std::map<GLchar, Character> characters;
    
    //OpenGL buffers
    GLuint VAO;
    GLuint VBO;
    
    //Shader to use when rendering
    Shader shader;
    
    //Private member functions
};

#endif /* Font_hpp */
