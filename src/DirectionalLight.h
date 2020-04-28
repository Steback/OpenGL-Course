#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity,GLfloat _dIntensity, GLfloat _xDir, GLfloat _yDir, GLfloat _zDir);
        ~DirectionalLight();
        void useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation) const;

    private:
        glm::vec3 direction;
};

#endif