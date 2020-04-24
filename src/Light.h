#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "../lib/glm/glm.hpp"

class Light {
    public:
        explicit Light(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity,
                GLfloat _xDir, GLfloat _yDir, GLfloat _zDir, GLfloat _dIntensity);
        ~Light();
        void useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation,
                GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const;

    private:
        glm::vec3 colour;
        GLfloat ambientIntensity;
        glm::vec3 direction;
        GLfloat diffuseIntensity;
};

#endif