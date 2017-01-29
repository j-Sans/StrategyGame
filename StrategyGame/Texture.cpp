//
//  Texture.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Texture.hpp"

//Public member functions
void Texture::set(const GLchar* imagePath, GLuint texNumber, const GLchar* uniformName) {
    if (texNumber >= 31) {
        texNumber = 30; //Stops bad access from accessing greater than element 30 in the because OpenGL might only be able to use 32 textures, and the last is reserved for fonts
    }
    
    this->id = GL_TEXTURE0 + texNumber;
    
    this->texNum = texNumber;
    
    this->name = uniformName;
    
    int textureWidth, textureHeight;
    unsigned char *image;
    
    //Load in the image for the container texture
    image = SOIL_load_image(imagePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
    
    //Make the texture
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Generate the image for the currently bound texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //Free the memory associated with the texture and unbind it
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, this->texNum);
    
    this->set = true;
}

//Activate the texture and send the information to the given shader
const void Texture::use(Shader shader) {
    if (!this->set)
        throw "Texture not set";
    
    glActiveTexture(this->id);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    shader.uniformTex(this->name.c_str(), this->texNum);
}
