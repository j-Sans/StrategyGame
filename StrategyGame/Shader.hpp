//
//  Shader.hpp
//  BaseOpenGL
//
//  Created by Jake Sanders on 6/6/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!Getting-started/Shaders by Joey De Vries
//  This has been altered from the original code, and is not endoresed nor endorsed by LearnOpenGL
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


/*!
 * A shader object that contains the compiled shader program. Can be used with a simple function and also can take different types of uniforms.
 *
 * @param vertexPath A c-string which is the path to the text file that contains vertex shader GLSL code.
 * @param geometryPath An optional c-string which is the path to the text file that contains geometry shader GLSL code. This parameter is not necessary.
 * @param fragmentPath A c-string which is the path to the text file that contains fragment shader GLSL code.
 */
class Shader {
public:
    //The program ID
    
    /*!
     * The gl reference to the compiled shader program.
     */
    GLuint program;
    
    //Default constructor. Don't use this, it is only to allow shader objects to exist in classes without being declared first.
    Shader();
    
    //Constructor that reads in and builds the shader with no geometry shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    //Constructor that reads in and builds the shader with a geometry shader
    Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
    
    /*!
     * Uses the shader object by running the compiled shader program. Used as "glUseProgram()" would be.
     */
    void use();
    
    //Send uniforms to the shader
    
    /*!
     * Send a texture to the shader program.
     *
     * @param name The name of the sampler2D in GLSL.
     * @param texNum The activation number of the texture, from 0-32. (This is used for GL_TEXTURE0 activtion, but with whichever number corresponds to the texture.
     */
    void uniformTex(const GLchar* name, GLuint texNum);
    
    /*!
     * Send a float to the shader program.
     *
     * @param name The name of the float in GLSL.
     * @param f The float to send.
     */
    void uniform1f(const GLchar* name, float f);
    
    /*!
     * Send a vector of length 2 to the shader program.
     *
     * @param name The name of the vec2 in GLSL.
     * @param vec The vector to send.
     */
    void uniform2f(const GLchar* name, glm::vec2 vec);
    
    /*!
     * Send a vector of length 3 to the shader program.
     *
     * @param name The name of the vec3 in GLSL.
     * @param vec The vector to send.
     */
    void uniform3f(const GLchar* name, glm::vec3 vec);
    
    /*!
     * Send a vector of length 4 to the shader program.
     *
     * @param name The name of the vec4 in GLSL.
     * @param vec The vector to send.
     */
    void uniform4f(const GLchar* name, glm::vec4 vec);
    
    /*!
     * Send a 2x2 matrix to the shader program.
     *
     * @param name The name of the mat2 in GLSL.
     * @param mat The matrix to send.
     */
    void uniformMat2(const GLchar* name, glm::mat2x2 mat);
    
    /*!
     * Send a 3x3 matrix to the shader program.
     *
     * @param name The name of the mat3 in GLSL.
     * @param mat The matrix to send.
     */
    void uniformMat3(const GLchar* name, glm::mat3x3 mat);
    
    /*!
     * Send a 4x4 matrix to the shader program.
     *
     * @param name The name of the mat4 in GLSL.
     * @param mat The matrix to send.
     */
    void uniformMat4(const GLchar* name, glm::mat4x4 mat);
};

#endif /* Shader_hpp */
