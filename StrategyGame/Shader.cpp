//
//  Shader.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Guided by the tutorial at http://learnopengl.com/#!Getting-started/Shaders by Joey De Vries
//  This has been altered from the original code, and is not endoresed nor endorsed by LearnOpenGL
//

#include "Shader.hpp"

//Only so that Visualizer.hpp can have a shader property without declaring it initially. No other purpose.
Shader::Shader() {}

//Constructor without the geometry shader
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    //Get the shader code:
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
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
    } catch (std::ifstream::failure e) {
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

//Constructor with the geometry shader
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
    try {
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
    } catch (std::ifstream::failure e) {
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

//Use the program
void Shader::use() {
    glUseProgram(this->program);
}

//Functions to send uniforms of various types directly to the shader
void Shader::uniformTex(const GLchar* name, GLuint texNum) {
    glUniform1i(glGetUniformLocation(this->program, name), texNum);
}

void Shader::uniform1i(const GLchar* name, int i) {
    glUniform1i(glGetUniformLocation(this->program, name), i);
}

void Shader::uniform1f(const GLchar* name, float f) {
    glUniform1f(glGetUniformLocation(this->program, name), f);
}

void Shader::uniform2f(const GLchar* name, glm::vec2 vec) {
    glUniform2f(glGetUniformLocation(this->program, name), vec.x, vec.y);
}

void Shader::uniform3f(const GLchar* name, glm::vec3 vec) {
    glUniform3f(glGetUniformLocation(this->program, name), vec.x, vec.y, vec.z);
}

void Shader::uniform4f(const GLchar* name, glm::vec4 vec) {
    glUniform4f(glGetUniformLocation(this->program, name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::uniformMat2(const GLchar* name, glm::mat2x2 mat) {
    glUniformMatrix2fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::uniformMat3(const GLchar* name, glm::mat3x3 mat) {
    glUniformMatrix3fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::uniformMat4(const GLchar* name, glm::mat4x4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(mat));
}