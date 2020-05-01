#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light  {
    public:
        PointLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity,
                GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLfloat _cons, GLfloat _lin, GLfloat _exp);
        ~PointLight();
        void useLight(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _positionLocation,
                GLfloat _constantLocation, GLfloat _linearLocation, GLfloat _exponentLocation) const;

    private:
        glm::vec3 position;
        GLfloat constant, linear, exponent;
};

#endif