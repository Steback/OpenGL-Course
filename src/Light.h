#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "../lib/glm/glm.hpp"

class Light {
    public:
        explicit Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity);
        ~Light();

    protected:
        glm::vec3 colour;
        GLfloat ambientIntensity;
        GLfloat diffuseIntensity;
};

#endif