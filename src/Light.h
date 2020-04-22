#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "../lib/glm/glm.hpp"

class Light {
    public:
        explicit Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity);
        ~Light();
        void useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation) const;

    private:
        glm::vec3 colour;
        GLfloat ambientIntensity;
};

#endif