//
//  SpriteRenderer.hpp
//  Game
//
//  Created by Jake Sanders on 6/24/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!In-Practice/2D-Game/Rendering-Sprites by Joey De Vries
//  This has been altered from the original code, and is not supported nor endorsed by LearnOpenGL
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Shader.h"

/**
 * Constructor for an object that renders sprites.
 *
 * @param shader A compiled shader program.
 */
class SpriteRenderer {
public:
    //Constructor
    SpriteRenderer(Shader shader);
    
    //Destructor
    ~SpriteRenderer();
    
    //Public member functions
    void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    
private:
    //Private properties
    Shader shader;
    GLuint quadVAO;
    
    //Private member functions
    void initRenderData();
};

//Public member functions
void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color) {
    this->shader.use();
    
    //Make model transformation matrix
    glm::mat4 model;
    
    //Add transformations
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));
    model = glm::scale(model, vec3(size, 1.0f));
    
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader.program, "SpriteColor"), 1, GL_FALSE, glm::value_ptr(color));
    
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

//Private member functions
void SpriteRenderer::initRenderData() {
    //Set VAO and VBO
    GLuint VBO;
    
    GLfloat vertices[] = {
        //  Position    Texture
        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  0.0f, 0.0f,
        
        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f
    };
    
    //Set up the VAO and VBO
    glGenVertexBufferArray(1, this->quadVAO);
    glGenBuffers(1, &VBO);
    
    //Give the VBO the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Tell OpenGL how to interpret the vertex data
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif /* SpriteRenderer_hpp */
