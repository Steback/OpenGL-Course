
#include "../lib/glm/gtc/matrix_transform.hpp"

#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, GLfloat _yaw, GLfloat _pitch, GLfloat _movementSpeed, GLfloat _turnSpeed)
    : position(_position), worldUp(_up), yaw(_yaw), pitch(_pitch), movementSpeed(_movementSpeed), turnSpeed(_turnSpeed), front(glm::vec3(0.0f, 0.0f, -1.0f)) {
    update();
}

Camera::~Camera() = default;

void Camera::KeyControl(const bool *_keys) {
    if ( _keys[GLFW_KEY_W] ) {
        position += front * movementSpeed;
    }

    if ( _keys[GLFW_KEY_S] ) {
        position -= front * movementSpeed;
    }

    if ( _keys[GLFW_KEY_D] ) {
        position -= right * movementSpeed;
    }
    if ( _keys[GLFW_KEY_A] ) {
        position += right * movementSpeed;
    }
}

glm::mat4 Camera::calculateViewMatrix() { return glm::lookAt(position, position + front, up); }

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}