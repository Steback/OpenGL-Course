#include "PointLight.h"

PointLight::PointLight(GLfloat _red, GLfloat _green, GLfloat _blue, GLfloat _ambientIntensity, GLfloat _dIntensity,GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLfloat _cons, GLfloat _lin, GLfloat _exp)
    : Light(_red, _green, _blue, _ambientIntensity, _dIntensity), position(glm::vec3(_xPos, _yPos, _zPos)), constant(_cons), linear(_lin), exponent(_exp) {  }

PointLight::~PointLight() = default;

void PointLight::useLisht(GLfloat _ambientIntensityLocation, GLfloat _ambienColourLocation, GLfloat _diffuseIntensityLocation,
        GLfloat _positionLocation, GLfloat _constantLocation, GLfloat _linearLocation, GLfloat _exponentLocation) const {
    // glUniform — Specify the value of a uniform variable for the current program object
    glUniform3f(_ambienColourLocation, colour.x, colour.y, colour.z);
    glUniform1f(_ambientIntensityLocation, ambientIntensity);
    glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(_positionLocation, position.x, position.y, position.z);
    glUniform1f(_constantLocation, constant);
    glUniform1f(_linearLocation, linear);
    glUniform1f(_exponentLocation, exponent);
}