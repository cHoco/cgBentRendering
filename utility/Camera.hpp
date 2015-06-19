#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovementDirection {
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
};

const glm::vec3 UP       = glm::vec3(0.0f, 1.0f, 0.0f);
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      = 0.0f;
const glm::vec3 FRONT    = glm::vec3(0.0f, 0.0f, -1.0f);
const GLfloat SPEED      = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM       = 45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 frontVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    glm::vec3 worldUpVec;
    // Eular Angles
    GLfloat yawAngle;
    GLfloat pitchAngle;
    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoomQuantity;

    Camera(glm::vec3 position) : Camera(position, UP, YAW, PITCH) {};

    Camera(glm::vec3 position, glm::vec3 up,
            GLfloat yaw, GLfloat pitch) : Camera(position, up, yaw, pitch, FRONT, SPEED, SENSITIVTY, ZOOM) {};

    // Constructor with vectors
    Camera(glm::vec3 position, glm::vec3 up,
            GLfloat yaw, GLfloat pitch,
            glm::vec3 front,
            GLfloat movementSpeed, GLfloat mouseSensitivity, GLfloat zoom);

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
            GLfloat upX, GLfloat upY, GLfloat upZ,
            GLfloat yaw, GLfloat pitch,
            glm::vec3 front,
            GLfloat movementSpeed, GLfloat mouseSensitivity, GLfloat zoom);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovementDirection direction, GLfloat deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

private:
    void updateCameraVectors();
};

#endif
