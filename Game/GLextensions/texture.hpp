//
//  texture.hpp
//  Game
//
//  Created by Jake Sanders on 6/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

class Texture {
public:
    //Constructor
    Texture(const GLchar* imagePath, GLuint texNumber);
    
    //Public member functions
    const void use(Shader shader, const GLchar* uniformName);
private:
    //Private properties
    GLuint tex;
    GLuint texNum;
    GLuint id;
};

Texture::Texture(const GLchar* imagePath, GLuint texNumber) {
    if (texNumber > 15) {
        texNumber = 15; //Stops bad access from accessing greater than element 15 in the size-16 array textures
    }
    
    id = GL_TEXTURE0 + texNumber;
    
    this->texNum = texNumber;
    
    int textureWidth, textureHeight;
    unsigned char *image;
    
    //Load in the image for the container texture
    image = SOIL_load_image(imagePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    
    //Make the texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Generate the image for the currently bound texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //Free the memory associated with the texture and unbind it
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, this->texNum);
}

void Texture::use(Shader shader, const GLchar* uniformName) {
    glActiveTexture(this->id);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    shader.uniformTex(uniformName, this->texNum);
}

#endif /* Texture_hpp */
