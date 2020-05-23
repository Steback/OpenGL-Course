#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "glm/glm.hpp"

class Light {
    public:
        explicit Light(const glm::vec3& _colour, GLfloat _aIntensity, GLfloat _dIntensity);
        ~Light();

    protected:
        glm::vec3 colour;
        GLfloat ambientIntensity;
        GLfloat diffuseIntensity;
};

#endif