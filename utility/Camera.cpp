#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 up,
        GLfloat yaw, GLfloat pitch,
        glm::vec3 front,
        GLfloat movSpeed, GLfloat mouseSens, GLfloat zoom)
{
    position = pos;
    worldUpVec = up;
    yawAngle = yaw;
    pitchAngle = pitch;
    frontVec = front;
    movementSpeed = movSpeed;
    mouseSensitivity = mouseSens;
    zoomQuantity = zoom;
    this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat upX, GLfloat upY, GLfloat upZ,
        GLfloat yaw, GLfloat pitch,
        glm::vec3 front,
        GLfloat movSpeed, GLfloat mouseSens, GLfloat zoom)
{
    position = glm::vec3(posX, posY, posZ);
    worldUpVec = glm::vec3(upX, upY, upZ);
    yawAngle = yaw;
    pitchAngle = pitch;
    frontVec = front;
    movementSpeed = movSpeed;
    mouseSensitivity = mouseSens;
    zoomQuantity = zoom;
    this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + frontVec, upVec);
}

void Camera::ProcessKeyboard(CameraMovementDirection direction, GLfloat deltaTime) {
    GLfloat velocity = movementSpeed * deltaTime;
    if (direction == LEFT)
        position -= rightVec * velocity;
    if (direction == RIGHT)
        position += rightVec * velocity;
    if (direction == FORWARD)
        position += frontVec * velocity;
    if (direction == BACKWARD)
        position -= frontVec * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yawAngle += xoffset;
    pitchAngle += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitchAngle > 89.0f)
            pitchAngle = 89.0f;
        if (pitchAngle < -89.0f)
            pitchAngle = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (zoomQuantity >= 1.0f && zoomQuantity <= 45.0f)
        zoomQuantity -= yoffset;
    if (zoomQuantity <= 1.0f)
        zoomQuantity = 1.0f;
    if (zoomQuantity >= 45.0f)
        zoomQuantity = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
    front.y = sin(glm::radians(pitchAngle));
    front.z = sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
    frontVec = glm::normalize(front);
    rightVec = glm::normalize(glm::cross(frontVec, worldUpVec));
    upVec = glm::normalize(glm::cross(rightVec, frontVec));
}
