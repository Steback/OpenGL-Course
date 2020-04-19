#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"

class Camera {
    public:
        Camera(glm::vec3 _position, glm::vec3 _up, GLfloat _yaw, GLfloat _pitch, GLfloat _movementSpeed, GLfloat _turnSpeed);
        ~Camera();
        void KeyControl(const bool* _keys, GLfloat _deltaTime);
        void MouseControl(GLfloat _xChange, GLfloat _ychange);
        glm::mat4 calculateViewMatrix();

    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        GLfloat yaw;
        GLfloat pitch;
        GLfloat movementSpeed;
        GLfloat turnSpeed;

        void update();
};

#endif
