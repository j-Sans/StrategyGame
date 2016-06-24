//
//  camera.h
//  BaseOpenGL
//
//  Created by Jake Sanders on 6/12/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//
//  Created by Joey De Vriel in http://learnopengl.com/#!Getting-started/Camera
//  Original code: http://learnopengl.com/code_viewer.php?type=header&code=camera
//  Some variables, comments, and styles have been altered

#ifndef camera_h
#define camera_h

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.025f;
const GLfloat ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera {
private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
    
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM) {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM) {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    /** Returns the view matrix calculated using Eular Angles and the LookAt Matrix
     *
     * @Return The matrix that is used to make the 3D view
     */
    glm::mat4 getViewMatrix() {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
    
    /** Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems), and called by GLFW window.
     *
     * Translates the camera forward, backward, left, and right
     *
     * @Param direction Direction camera should be translated
     * @Param deltaTime Change in time since last call. Used to ensure that on all systems, camera speed is always constant.
     * @Param noFly prevents the camera from moving off of the ground
     */
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime, bool noFly) {
        GLfloat velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD)
            this->position += this->front * velocity;
        if (direction == BACKWARD)
            this->position -= this->front * velocity;
        if (direction == LEFT)
            this->position -= this->right * velocity;
        if (direction == RIGHT)
            this->position += this->right * velocity;
        
        if (noFly)
            this->position.y = 0.0f;
    }
    
    /** Processes input received from a mouse input system. Expects the offset value in both the x and y direction, and called by GLFW window.
     *
     * Swivels the camera vertically and horizontally
     *
     * @Param deltaX Distance mouse moved in x direction
     * @Param deltaY Distance mouse moved in y direction
     * @Param contrainPitch A boolean to declare if the pitch should be restrained between 89º and -89º. Automatically set to true.
     */
    void processMouseMovement(GLfloat deltaX, GLfloat deltaY, GLboolean constrainPitch = true) {
        deltaX *= this->mouseSensitivity;
        deltaY *= this->mouseSensitivity;
        
        this->yaw += deltaX;
        this->pitch += deltaY;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }
        
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }
    
    /** Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis, and called by GLFW window.
     *
     * Zooms in by decreasing the field of view region
     *
     * @Param deltaY Distance scrolled in y direction
     */
    void processMouseScroll(GLfloat deltaY) {
        if (this->zoom >= 1.0f && this->zoom <= 45.0f)
            this->zoom -= deltaY;
        if (this->zoom <= 1.0f)
            this->zoom = 1.0f;
        if (this->zoom >= 45.0f)
            this->zoom = 45.0f;
    }
};

#endif /* camera_h */
