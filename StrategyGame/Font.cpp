//
//  Font.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/27/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!In-Practice/Text-Rendering by Joey De Vries
//  This has been altered from the original code, and is not endoresed nor endorsed by LearnOpenGL
//

#include "Font.hpp"

//Only so that Visualizer.hpp can have a font property without declaring it initially. No other purpose.
Font::Font() {}

Font::Font(const char* fontPath) {
    
    //Load the library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "Error initializing FreeType font library" << std::endl;
    
    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
        std::cout << "Error loading font" << std::endl;
    
    //Setting the width to 0 means the width can be automatically determined
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        std::cout << "Error loading font glyph" << std::endl;
    
    
    //Store the characters as part of the font
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable byte-alignment restriction
    
    //Go through the characters and create each one and store it.
    for (GLubyte a = 0; a < 128; a++) {
        //Load character glyph
        if (FT_Load_Char(face, a, FT_LOAD_RENDER)) {
            std::cout << "Error loading font glyph" << std::endl;
            continue;
        }
        
        //Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        
        //Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        //Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x,
        };
        
        this->characters.insert(std::pair<GLchar, Character>(a, character));
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    //Set the data up with the OpenGL buffers
    
    //Generate the buffers
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    
    //Bind the VAO
    glBindVertexArray(this->VAO);
    
    //Bind the VBO and send it the data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); //6 * 4 because there are 6 vertices drawn for a rectangle (two are overlaps) and they each have 4 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    
    //Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //Set the shader
    this->shader = Shader("Shaders/font.vert", "Shaders/font.frag");
}

void Font::render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, glm::mat4 projection) {
    //Activate corresponding render state
    this->shader.use();
    
    this->shader.uniform3f("textColor", glm::vec3(color.x, color.y, color.z));
    
    glActiveTexture(GL_TEXTURE31);
    glBindVertexArray(VAO);
    
    //Iterate through all characters
    for (std::string::const_iterator a = text.begin(); a != text.end(); a++) {
        Character ch = characters[*a];
        
        GLfloat xPos = x + ch.bearing.x * scale;
        GLfloat yPos = y - (ch.size.y - ch.bearing.y) * scale;
        
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        
        //Update VBO for each character
        GLfloat vertices[6][4] = {
            {xPos, yPos + h, 0.0, 0.0},
            {xPos, yPos, 0.0, 1.0},
            {xPos + w, yPos, 1.0, 1.0},
            
            {xPos, yPos + h, 0.0, 0.0},
            {xPos + w, yPos, 1.0, 1.0},
            {xPos + w, yPos + h, 1.0, 0.0}
        };
        
        //Render glyph texture over quad
        
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        
        //Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}