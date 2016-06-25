//
//  Shader.hpp
//  BaseOpenGL
//
//  Created by Jake Sanders on 6/6/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!Getting-started/Shaders by Joey De Vries
//  This has been altered from the original code, and is not supported nor endorsed by LearnOpenGL
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>


/**
 * Constructor for a shader object that creates and compiles a shader program from text files containing shader code.
 *
 * @param vertexPath A c-string which is the path to the text file that contains vertex shader GLSL code.
 * @param geometryPath A c-string which is the path to the text file that contains geometry shader GLSL code. This parameter is optional.
 * @param fragmentPath A c-string which is the path to the text file that contains fragment shader GLSL code.
 */
class Shader {
public:
    //The program ID
    
    /**
     * The gl reference to the compiled shader program.
     */
    GLuint program;
    
    //Constructor that reads in and builds the shader with no geometry shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    //Constructor that reads in and builds the shader with a geometry shader
    Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
    
    //Use the program
    void use();
    
    //Send uniforms to the shader
    void uniform1f(const GLchar* name, float f) { glUniform1f(glGetUniformLocation(this->program, name), f); }
    void uniform2f(const GLchar* name, glm::vec2 vec) { glUniform2f(glGetUniformLocation(this->program, name), vec.x, vec.y); }
    void uniform3f(const GLchar* name, glm::vec3 vec) { glUniform3f(glGetUniformLocation(this->program, name), vec.x, vec.y, vec.z); }
    void uniform4f(const GLchar* name, glm::vec4 vec) { glUniform4f(glGetUniformLocation(this->program, name), vec.x, vec.y, vec.z, vec.w); }
    
    void uniformMat2(const GLchar* name, glm::mat2x2 mat) { glUniformMatrix2fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat)); }
    void uniformMat3(const GLchar* name, glm::mat3x3 mat) { glUniformMatrix3fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat)); }
    void uniformMat4(const GLchar* name, glm::mat4x4 mat) { glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat)); }
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    //Get the shader code:
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        
        // Convert stream into GLchar array
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "Error reading in shaders from file.\n";
    }
    
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    //Compile the shaders:
    //Create the vertex shader and attach to it the code above
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    
    //Check for errors compiling the shader
    GLint vertexSuccess;
    GLchar vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (!vertexSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
        std::cout << "Error compiling vertex shader: " << std::endl << vertexInfoLog << std::endl;
    }
    
    //Now do the same with the fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    
    //Check for errors again
    GLint fragmentSuccess;
    GLchar fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!fragmentSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
        std::cout << "Error compiling fragment shader: " << std::endl << fragmentInfoLog << std::endl;
    }
    
    //Shader program
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);
    
    //Check for errors linking the shaders
    GLint linkSuccess;
    GLchar linkInfoLog[512];
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(this->program, 512, NULL, linkInfoLog);
        std::cout << "Error linking shaders: " << std::endl << linkInfoLog << std::endl;
    }
    
    //Delete the shaders now that they are no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


Shader::Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath) {
    //Get the shader code:
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;
    
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        gShaderFile.open(geometryPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, gShaderStream, fShaderStream;
        
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // close file handlers
        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();
        
        // Convert stream into GLchar array
        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "Error reading in shaders from file.\n";
    }
    
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    //Compile the shaders:
    //Create the vertex shader and attach to it the code above
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    
    //Check for errors compiling the shader
    GLint vertexSuccess;
    GLchar vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (!vertexSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
        std::cout << "Error compiling vertex shader: " << std::endl << vertexInfoLog << std::endl;
    }
    
    //Now do the same with the geometry shader
    GLuint geometryShader;
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &gShaderCode, NULL);
    glCompileShader(geometryShader);
    
    //Check for errors again
    GLint geometrySuccess;
    GLchar geometryInfoLog[512];
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometrySuccess);
    if (!geometrySuccess) {
        glGetShaderInfoLog(geometryShader, 512, NULL, geometryInfoLog);
        std::cout << "Error compiling geometry shader: " << std::endl << geometryInfoLog << std::endl;
    }
    
    //And with the fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    
    //Check for errors again
    GLint fragmentSuccess;
    GLchar fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!fragmentSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
        std::cout << "Error compiling fragment shader: " << std::endl << fragmentInfoLog << std::endl;
    }
    
    //Shader program
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, geometryShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);
    
    //Check for errors linking the shaders
    GLint linkSuccess;
    GLchar linkInfoLog[512];
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(this->program, 512, NULL, linkInfoLog);
        std::cout << "Error linking shaders: " << std::endl << linkInfoLog << std::endl;
    }
    
    //Delete the shaders now that they are no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

/**
 * Uses the shader object by running the compiled shader program. Used as "glUseProgram()" would be.
 */
void Shader::use() {
    glUseProgram(this->program);
}

#endif /* Shader_hpp */
