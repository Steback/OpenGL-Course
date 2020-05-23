#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, GLfloat _yaw, GLfloat _pitch, GLfloat _movementSpeed, GLfloat _turnSpeed)
    : position(_position), worldUp(_up), yaw(_yaw), pitch(_pitch), movementSpeed(_movementSpeed), turnSpeed(_turnSpeed), front(glm::vec3(0.0f, 0.0f, -1.0f)) {
    update();
}

Camera::~Camera() = default;

void Camera::KeyControl(const bool *_keys, GLfloat _deltaTime) {
    GLfloat velocity = movementSpeed * _deltaTime;

    if ( _keys[GLFW_KEY_W] ) {
        position += front * velocity;
    }

    if ( _keys[GLFW_KEY_S] ) {
        position -= front * velocity;
    }

    if ( _keys[GLFW_KEY_A] ) {
        position -= right * velocity;
    }
    if ( _keys[GLFW_KEY_D] ) {
        position += right * velocity;
    }
}

void Camera::MouseControl(GLfloat _xChange, GLfloat _ychange) {
    _xChange *= turnSpeed;
    _ychange *= turnSpeed;

    yaw += _xChange;
    pitch += _ychange;

    if ( pitch > 89.0f ) pitch = 89.0f;
    if ( pitch < -89.0f ) pitch = -89.0f;

    update();
}

glm::mat4 Camera::calculateViewMatrix() { return glm::lookAt(position, position + front, up); }

glm::vec3 Camera::getCameraPosition() { return position; }

glm::vec3 Camera::getCameraDirection() { return glm::normalize(front); }

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // Returns a vector in the same direction as x but with length of 1.
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}